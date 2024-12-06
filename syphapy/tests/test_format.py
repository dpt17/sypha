# test_format.py
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

from syphapy.format import Formatter, FormatType
import filecmp

logger = logging.getLogger(__name__)

def test_infer_no_extension():
    format_type = Formatter.infer_format_type("no_file_extension")
    assert format_type is None

def test_infer_csv_extension():
    format_type = Formatter.infer_format_type("a_reallyyyyyyyyyyyyyyyyyyyyy_long_file_name.csv")
    assert format_type == FormatType.CSV

def test_infer_parquet_extension():
    format_type = Formatter.infer_format_type("short.parquet")
    assert format_type == FormatType.PARQUET

def test_infer_wacky_file_name():
    format_type = Formatter.infer_format_type("a.really.complicated-yet_should.work-anyway_fileName.parquet")
    assert format_type == FormatType.PARQUET

def test_csv_to_parquet():
    formatter = Formatter.load_file("tests/data/sample2.csv", FormatType.CSV)
    formatter.dump_file("tests/data/sample2_a.parquet", FormatType.PARQUET)
    
    assert filecmp.cmp("tests/data/sample2.parquet", "tests/data/sample2_a.parquet", shallow=False)

def test_parquet_to_csv():
    formatter = Formatter.load_file("tests/data/sample2.parquet", FormatType.PARQUET)
    formatter.dump_file("tests/data/sample2_a.csv", FormatType.CSV)
    
    assert filecmp.cmp("tests/data/sample2.csv", "tests/data/sample2_a.csv", shallow=False)

def test_csv_to_csv():
    formatter = Formatter.load_file("tests/data/sample2.csv", FormatType.CSV)
    formatter.dump_file("tests/data/sample2_b.csv", FormatType.CSV)
    
    assert filecmp.cmp("tests/data/sample2.csv", "tests/data/sample2_b.csv", shallow=False)

def test_parquet_to_parquet():
    formatter = Formatter.load_file("tests/data/sample2.parquet", FormatType.PARQUET)
    formatter.dump_file("tests/data/sample2_b.parquet", FormatType.PARQUET)
    
    assert filecmp.cmp("tests/data/sample2.parquet", "tests/data/sample2_b.parquet", shallow=False)
