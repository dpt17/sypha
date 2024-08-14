/* env.c
 *
 * Copyright 2024 David Tuttle
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "syphac/sypha_env.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SZ           10240
#define ENV_FILE            ".env"
#define ASSIGNMENT_TOKEN    '='
#define CR_TOKEN            '\r'
#define LF_TOKEN            '\n'
#define TAB_TOKEN           '\t'
#define SPACE_TOKEN         ' '

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

void sypha_env_load_dot_env() {
    char buffer[BUFFER_SZ];
    size_t element_sz = sizeof(char);
    char key[MAX_KEY_LEN + 1];
    char value[MAX_VALUE_LEN + 1];
    int key_byte_count, value_byte_count;
    FILE * file;
    size_t bytes_read;
    int in_key;
    char * buffer_ptr, * key_ptr, * value_ptr;
    char c;
    int i;

    if (!(file = fopen (ENV_FILE, "r"))) {
        // unable to load env --> this could be fine if there is no .env file
        return;
    }
    
    // Set initial state
    in_key = 1;
    key_byte_count = 0;
    value_byte_count = 0;
    key_ptr = key;
    value_ptr = value;

    do {
        bytes_read = fread(buffer, element_sz, BUFFER_SZ, file);
        for (buffer_ptr = buffer, i = 0; i < bytes_read; buffer_ptr++, i++) {
            c = *buffer_ptr;
            switch (c) {
                case ASSIGNMENT_TOKEN:
                    if (in_key) {
                        // finish up and continue
                        in_key = 0;
                    } else {
                        // write it as normal ... we can allow '=' in a value I suppose
                        if (value_byte_count < MAX_VALUE_LEN) {
                            *value_ptr = c;
                            value_ptr++;
                            value_byte_count++;
                        }
                        // else: we reached limit so ignore absusive line in file
                    }
                    break;
                case CR_TOKEN:
                    // Don't do anything.  We're likely on Windows with a '\n' on deck.  Yes, this would
                    // break a *nix system that had a '\r' in a KVP but is that even a real scenario?
                    break;
                case LF_TOKEN:
                    if (in_key) {
                        // broken KVP or maybe just whitespace in the file, skip and continue on ...
                    } else {
                        // write KVP
                        key[key_byte_count] = '\0';
                        value[value_byte_count] = '\0';
                        if (setenv(key, value, 1) < 0) {
                            // TODO: handle error?
                        }
                    }

                    // reset
                    in_key = 1;
                    key_ptr = key;
                    key_byte_count = 0;
                    value_ptr = value;
                    value_byte_count = 0;
                    break;
                case TAB_TOKEN:
                case SPACE_TOKEN:
                    // TODO: need special handling for these as a key?
                default:
                    // A random char in either feed
                    if (in_key) {
                        if (key_byte_count < MAX_KEY_LEN) {
                            *key_ptr = c;
                            key_ptr++;
                            key_byte_count++;
                        }
                    } else {
                        if (value_byte_count < MAX_VALUE_LEN) {
                            *value_ptr = c;
                            value_ptr++;
                            value_byte_count++;
                        }
                    }
                    break;
            }
        }
    } while (bytes_read == BUFFER_SZ);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */
