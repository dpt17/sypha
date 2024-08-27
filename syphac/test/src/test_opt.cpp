/* test_opt.cpp
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

#include <stdlib.h>
#include <string.h>
#include "doctest.h"
#include "syphac/sypha_opt.h"

#define ARG_MAX_LEN     64
#define ARG_COUNT_MAX   16

TEST_CASE("Happy Path CLI") {

    char * argv[ARG_COUNT_MAX];
    for (int i=0;i < ARG_COUNT_MAX; i++) {
        argv[i] = (char *) malloc(sizeof(char) * ARG_MAX_LEN);
        *(argv[i]) = '\0';
    }

    SUBCASE("Happy Path Config") {
        strcpy(argv[0], "my_program");
        strcpy(argv[1], "-h");
        strcpy(argv[2], "localhost");
        strcpy(argv[3], "--port");
        strcpy(argv[4], "12345");
        int argc = 5;

        SYPHA_OPT_CONFIG opt_config;
        REQUIRE((opt_config = sypha_opt_config_add_param(NULL, "-f", "--force", 1, 0)) != NULL);
        CHECK(sypha_opt_config_add_param(opt_config, "-h", "--host", 0, 1) != NULL);
        CHECK(sypha_opt_config_add_param(opt_config, "-p", "--port", 0, 1) != NULL);

        SUBCASE("Happy Path Result") {
            SYPHA_OPT_PARSE_RESULT opt_parse_result = sypha_opt_parse_args(opt_config, argc, argv);
            REQUIRE(opt_parse_result != NULL);

            CHECK_EQ(strcmp(sypha_opt_parse_get_value(opt_parse_result, "--host"), "localhost"), 0);
            CHECK_EQ(strcmp(sypha_opt_parse_get_value(opt_parse_result, "-p"), "12345"), 0);

            // Note: implicitly tests that --force is indeed optional

            sypha_opt_parse_free(opt_parse_result);
        }

        SUBCASE("Missing required param") {
            CHECK(sypha_opt_config_add_param(opt_config, "-z", "--sleep", 0, 1) != NULL);

            SYPHA_OPT_PARSE_RESULT opt_parse_result = sypha_opt_parse_args(opt_config, argc, argv);
            CHECK(opt_parse_result == NULL);
        }

        SUBCASE("Flag") {
            strcpy(argv[5], "--force");
            argc = 6;

            SYPHA_OPT_PARSE_RESULT opt_parse_result = sypha_opt_parse_args(opt_config, argc, argv);
            REQUIRE(opt_parse_result != NULL);
        }

        sypha_opt_config_free(opt_config);
    }

    for (int i = 0; i < ARG_COUNT_MAX; i++) {
        free(argv[i]);
    }
}
