#!/bin/bash

# convert.sh
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

SRC=""
DST=""

print_usage() {
    echo "Usage: $0 -i src_file -o dst_file"
}

while getopts ":i:o:" option; do
  case $option in
    i)
      SRC="$OPTARG"
      ;;
    o)
      DST="$OPTARG"
      ;;
    *)
      print_usage
      exit 1
      ;;
  esac
done

if [ "$SRC" == "" ]; then
    print_usage
    exit 1
fi

if [ "$DST" == "" ]; then
    print_usage
    exit 1
fi

SRC=$(realpath $SRC)
DST=$(realpath $DST)

poetry run python main.py --input $SRC --output $DST
