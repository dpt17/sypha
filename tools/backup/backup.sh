#!/bin/bash

# backup.sh
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

# TODO: add logfile option or just leave as stdout?

SRC=""
DST=""
DELETE=0

print_usage() {
    echo "Usage: $0 -i src_dir -o dst_dir [-d]"
}

while getopts ":i:o:d" option; do
  case $option in
    i)
      SRC="$OPTARG"
      ;;
    o)
      DST="$OPTARG"
      ;;
    d)
      DELETE=1
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

if [ $DELETE == 1 ]; then
    echo -e "\nbackup.sh ... backing up contents of ${SRC} to ${DST}, deleting missing in ${DST}"
else
    echo -e "\nbackup.sh ... backing up contents of ${SRC} to ${DST}, not deleting missing in ${DST}"
fi

START=$(date)
echo -e "\nStart: ${START}\n"

cd $SRC
if [ $DELETE == 1 ]; then
    rsync -avP . $DST --delete
else
    rsync -avP . $DST
fi

END=$(date)
echo -e "\nEnd: ${END}\n"
