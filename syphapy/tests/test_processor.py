# test_processor.py
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

import logging

import time
from typing import Sequence
from syphapy.processor import QueueProcessor, Item
from threading import RLock

logger = logging.getLogger(__name__)

class NoneQueueProcessor(QueueProcessor):
    def __init__(self, capacity=16, producer_count=1, producer_throttle=0.1, consumer_count=1, consumer_throttle=0.1):
        super().__init__(capacity, producer_count, producer_throttle, consumer_count, consumer_throttle)
        self.processed_count = 0

    def produce(self, producer_tid: int) -> Sequence[Item] | None:
        logger.debug(f"produce - thread {producer_tid}: None")
        return None

    def consume(self, consumer_tid: int, item: Item):
        logger.debug(f"consume - thread {consumer_tid}: {item}")
        self.processed_count = self.processed_count + 1

class EmptyQueueProcessor(QueueProcessor):
    def __init__(self, capacity=16, producer_count=1, producer_throttle=0.1, consumer_count=1, consumer_throttle=0.1):
        super().__init__(capacity, producer_count, producer_throttle, consumer_count, consumer_throttle)
        self.processed_count = 0

    def produce(self, producer_tid: int) -> Sequence[Item] | None:
        result = list()
        if producer_tid % 2 > 0:
            item: Item = {}
            item["foo"] = "bar"
            result.append(item)
        
        logger.debug(f"producer - thread {producer_tid}: {result}")

        return result

    def consume(self, consumer_tid: int, item: Item):
        logger.debug(f"consume - thread {consumer_tid}: {item}")
        self.processed_count = self.processed_count + 1

class MultiQueueProcessor(QueueProcessor):
    def __init__(self, capacity=16, producer_count=5, producer_throttle=0.1, consumer_count=7, consumer_throttle=0.1):
        super().__init__(capacity, producer_count, producer_throttle, consumer_count, consumer_throttle)
        self.producer_lock = RLock()
        self.event_count = 0
        self.consumer_lock = RLock()
        self.processed_count = 0

    def produce(self, producer_tid: int) -> Sequence[Item] | None:
        result = list()
        if producer_tid % 2 > 0:
            item: Item = {}
            item["foo"] = "bar"
            result.append(item)
        
        logger.debug(f"producer - thread {producer_tid}: {result}")

        # They all synchronize on this so not great but this is test code
        item_count = len(result)
        if item_count > 0:
            with self.producer_lock:
                self.event_count = self.event_count + item_count

        return result

    def consume(self, consumer_tid: int, item: Item):
        logger.debug(f"consume - thread {consumer_tid}: {item}")

        # They all synchronize on this so not great but this is test code
        with self.consumer_lock:
            self.processed_count = self.processed_count + 1

def test_stop_by_producer_return_none():
    processor = NoneQueueProcessor()
    processor.start_all()

    # Note: no explicit stop of producers
    processor.join_producers()
    processor.stop_consumers()
    processor.join_consumers()

    assert processor.processed_count == 0

def test_stop_explicit_call():
    processor = EmptyQueueProcessor()
    processor.start_all()
    processor.stop_all()
    assert processor.processed_count == 0

def test_producer_return_empty_list():
    processor = EmptyQueueProcessor(producer_count=2)
    processor.start_all()
    processor.stop_all()
    assert processor.processed_count > 0

def test_multi_producer_and_consumer():
    processor = MultiQueueProcessor()
    processor.start_all()
    processor.stop_all()
    assert processor.event_count == processor.processed_count

def test_stress():
    processor = MultiQueueProcessor(capacity=20480, producer_count=32, producer_throttle=0.1, consumer_count=16, consumer_throttle=0.1)
    processor.start_all()
    time.sleep(3)
    processor.stop_all()
    assert processor.event_count == processor.processed_count
