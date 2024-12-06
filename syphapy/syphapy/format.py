# format.py
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

# Utility code to ingest data in one format and export it in another format

from __future__ import annotations

from enum import Enum
import pandas as pd
import pyarrow as pa
import logging

logger = logging.getLogger(__name__)

CSV_EXT = "csv"
PARQUET_EXT = "parquet"

class FormatType(Enum):
    CSV = 1
    PARQUET = 2

class Formatter:
    def __init__(self, df_obj: pd.DataFrame = None):
        self.df_obj : pd.DataFrame = df_obj

    @staticmethod
    def infer_format_type(file_path : str) -> FormatType | None:
        file_parts = file_path.split(".")
        
        if file_parts is None:
            return None
        
        file_parts_count = len(file_parts)
        if file_parts_count < 2:
            return None
        
        file_type = file_parts[file_parts_count - 1]
        file_type = file_type.lower()

        if file_type == CSV_EXT:
            return FormatType.CSV
        elif file_type == PARQUET_EXT:
            return FormatType.PARQUET
        else:
            return None

    @staticmethod
    def load_file(input_path: str, format_type: FormatType = None) -> Formatter | None:
        result : Formatter
        match format_type:
            case FormatType.CSV:
                # Note: skipped index_col=False param as we are handling this on output
                df : pd.DataFrame = pd.read_csv(input_path)
                result = Formatter(df)
            case FormatType.PARQUET:
                df : pd.DataFrame = pd.read_parquet(input_path, engine="pyarrow")
                result = Formatter(df)
            case _:
                format_type = Formatter.infer_format_type(input_path)

                if format_type is not None:
                    result = Formatter.load_file(input_path, format_type)
                else:
                    logger.error("load_file - unknown file type")
                    result = None
        
        return result

    def dump_file(self, output_path: str, format_type: FormatType = None):
        match format_type:
            case FormatType.CSV:
                # Don't output an extra index columns that DataFrame may have added internally
                self.df_obj.to_csv(output_path, index=False)
            case FormatType.PARQUET:
                # Don't output an extra index columns that DataFrame may have added internally
                self.df_obj.to_parquet(output_path, engine="pyarrow", index=False)
            case _:
                format_type = Formatter.infer_format_type(input_path)

                if format_type is not None:
                    self.dump_file(output_path, format_type)
                else:
                    logger.error("load_file - unknown file type")
