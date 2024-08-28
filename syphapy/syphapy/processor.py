# queue.py
#
# Copyright 2024 David Tuttle
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# An attempt at generalizing a pattern of having a queue and spinning up a set of
# worker threads to drain the queue.

from abc import ABC, abstractmethod
from queue import Queue
from threading import Thread, Event
from typing import Sequence, List

POISON = "poison"

class QueueProcessor(ABC):
    def __init__(self, producer_count=1, consumer_count=1):
        self.__queue = Queue()
        
        self.__producer_count=producer_count
        self.__producers = List[Thread]
        for i in range(self.__producer_count):
            self.__producers.append(Producer(i, self))
        
        self.__consumer_count=consumer_count
        self.__consumers = List[Thread]
        for i in range(self.__consumer_count):
            self.__consumers.append(Consumer(i, self))

    # Child class should implement this such that each call to this by a
    # producer thread yields N queue items
    @abstractmethod
    def produce(self, producer_tid: int) -> Sequence[dict]:
        pass

    # Child class should implement this such that each call to this by a
    # worker thread handles 1 queue item
    @abstractmethod
    def consume(self, consumer_tid: int, item: dict):
        pass

    def start(self):
        # Start producer threads
        for i in range(self.__producer_count):
            self.__producers[i].start()

        # Start consumer threads
        for i in range(self.__consumer_count):
            self.__consumers[i].start()

    # Gracefully stops the processor, leaving no pulled items unprocessed
    def stop(self):
        # Signal stop to all producers
        for i in range(self.__producer_count):
            self.__producers[i].stop()

        # Wait for producers to be stopped
        for i in range(self.__producer_count):
            self.__producers[i].join()

        # Signal stop to all consumers
        for i in range(self.__consumer_count):
            self.__queue.put({ POISON: 1 })

        # Wait for consumers to be stopped
        for i in range(self.__producer_count):
            self.__consumers[i].join()


# Stops trying to pull new work items when signaled to do so
class Producer(Thread):
    def __init__(self, tid: int, processor: QueueProcessor):
        self.tid = tid
        self.processor = processor
        self.event = Event()

    def run(self):
        while not self.event.is_set():
            # This cannot block, must give up after wait and yield no items
            items = self.processor.produce(self.tid)
            for item in items:
                # Blocks, which is ok
                self.processor.__queue.put(item)
    
    def stop(self):
        self.event.set()

# Doesn't stop handling work items until receiving a specific message in the work queue
class Consumer(Thread):
    def __init__(self, tid: int, processor: QueueProcessor):
        self.tid = tid
        self.processor = processor

    def run(self):
        while True:
            # This can block, which is ok since we are pushing enough poison messages
            # for each consumer thread to get one.
            item = self.processor.__queue.get()
            
            # Time to stop?
            poison = item.get(POISON, None)
            if poison != None:
                break
            
            self.processor.consume(self.tid, item=item)
