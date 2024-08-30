# processor.py
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
import time
import logging

logger = logging.getLogger(__name__)

POISON = "poison"

class Item(dict):
    pass
    
class QueueProcessor(ABC):
    # The counts are the number of respective worker thread to create.  The throttles are the number of
    # seconds to sleep for if needing to slow down activity (for whatever reason)
    def __init__(self, capacity=0, producer_count=1, producer_throttle=0.1, consumer_count=1, consumer_throttle=0.1):
        self._queue = Queue(maxsize=capacity)
        
        self.__producer_count=producer_count
        self.__producers: List[Producer] = []
        for i in range(self.__producer_count):
            self.__producers.append(Producer(i, producer_throttle, self))
        
        self.__consumer_count=consumer_count
        self.__consumers: List[Consumer] = []
        for i in range(self.__consumer_count):
            self.__consumers.append(Consumer(i, consumer_throttle, self))

    # Child class should implement this such that each call to this by a
    # producer thread yields N queue items
    #
    # Returns a list of zero or more Items or None if there is nothing left
    # and the producer should halt.  A list of length zero means there are
    # no more items at this time but we want to continue and wait for more.
    @abstractmethod
    def produce(self, producer_tid: int) -> Sequence[Item] | None:
        pass

    # Child class should implement this such that each call to this by a
    # worker thread handles 1 queue item
    @abstractmethod
    def consume(self, consumer_tid: int, item: Item):
        pass

    # Starts producer threads
    def start_producers(self):
        logger.debug("Starting producers")

        # Start producer threads
        for i in range(self.__producer_count):
            self.__producers[i].start()

    # Starts consumer threads
    def start_consumers(self):
        logger.debug("Starting consumers")

        # Start consumer threads
        for i in range(self.__consumer_count):
            self.__consumers[i].start()

    # Starts all producer and consumer threads
    def start_all(self):
        self.start_producers()
        self.start_consumers()

    # Stop producer threads
    def stop_producers(self):
        logger.debug("Stopping producers")

        # Signal stop to all producers
        for i in range(self.__producer_count):
            self.__producers[i].stop()

    # Wait for producer threads to finish (blocks!)
    def join_producers(self):
        logger.debug("Join producers")

        # Wait for producers to be stopped
        for i in range(self.__producer_count):
            self.__producers[i].join()

    # Stop consumer threads
    def stop_consumers(self):
        logger.debug("Stopping consumers")

        # Signal stop to all consumers
        for _ in range(self.__consumer_count):
            self._queue.put({ POISON: 1 })

    # Wait for consumer threads to finish (blocks!)
    def join_consumers(self):
        logger.debug("Join consumers")

        # Wait for consumers to be stopped
        for i in range(self.__consumer_count):
            self.__consumers[i].join()

    # Gracefully stops the processor, leaving no pulled items unprocessed
    # Blocks until all workers stop.
    def stop_all(self):
        self.stop_producers()
        self.join_producers()
        self.stop_consumers()
        self.join_consumers()

# Stops trying to pull new work items when signaled to do so
class Producer(Thread):
    def __init__(self, tid: int, throttle: float, processor: QueueProcessor):
        super().__init__()

        self.tid = tid
        self.throttle = throttle
        self.processor = processor
        self.event = Event()

        logger.debug(f"Producer thread {self.tid} created")

    def run(self):
        logger.debug(f"Producer thread {self.tid} starting")

        # While not signaled to stop via stop(), attempt to pull more items
        while not self.event.is_set():
            # This cannot block, must give up after wait and yield no items or None
            items = self.processor.produce(self.tid)
            
            if items is None:
                # A signal to halt because there are no more expected items
                logger.debug(f"Producer thread {self.tid} received None")
                break

            if len(items) > 0:
                for item in items:
                    # Blocks, which is ok
                    self.processor._queue.put(item)
            else:
                logger.debug(f"Producer thread {self.tid} received empty list, throttle")
                time.sleep(self.throttle)
    
        logger.debug(f"Producer thread {self.tid} exiting")

    def stop(self):
        logger.debug(f"Producer thread {self.tid} set stop signal")
        self.event.set()

# Doesn't stop handling work items until receiving a specific message in the work queue
class Consumer(Thread):
    def __init__(self, tid: int, throttle: float, processor: QueueProcessor):
        super().__init__()

        self.tid = tid
        self.throttle = throttle
        self.processor = processor

        logger.debug(f"Consumer thread {self.tid} created")

    def run(self):
        logger.debug(f"Consumer thread {self.tid} starting")
        while True:
            # This can block, which is ok since we are pushing enough poison messages
            # for each consumer thread to get one.
            item = self.processor._queue.get()
            
            # Time to stop?
            poison = item.get(POISON, None)
            if poison != None:
                logger.debug(f"Consumer thread {self.tid} poisoned")
                break
            
            self.processor.consume(self.tid, item=item)
        
        logger.debug(f"Consumer thread {self.tid} exiting")
