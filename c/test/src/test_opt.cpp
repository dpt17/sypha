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

TEST_CASE("Test opt") {

    int argc = 5;
    char * argv[5];
    argv[0] = (char *) malloc(sizeof(char) * 64);
    strcpy(argv[0], "my_program");
    argv[1] = (char *) malloc(sizeof(char) * 64);
    strcpy(argv[1], "-h");
    argv[2] = (char *) malloc(sizeof(char) * 64);
    strcpy(argv[2], "localhost");
    argv[3] = (char *) malloc(sizeof(char) * 64);
    strcpy(argv[3], "--port");
    argv[4] = (char *) malloc(sizeof(char) * 64);
    strcpy(argv[4], "12345");

    SUBCASE("Happy Path Config") {
        SYPHA_OPT_CONFIG opt_config;
        REQUIRE((opt_config = sypha_opt_config_add_param(NULL, "-f", "--force", 1, 0)) != NULL);
        CHECK(sypha_opt_config_add_param(opt_config, "-h", "--host", 0, 1) != NULL);
        CHECK(sypha_opt_config_add_param(opt_config, "-p", "--port", 0, 1) != NULL);

        SUBCASE("Happy Path Result") {
            SYPHA_OPT_PARSE_RESULT opt_parse_result = sypha_opt_parse_args(opt_config, argc, argv);
            REQUIRE(opt_parse_result != NULL);

            CHECK_EQ(strcmp(sypha_opt_parse_get(opt_parse_result, "--host"), "localhost"), 0);
            CHECK_EQ(strcmp(sypha_opt_parse_get(opt_parse_result, "-p"), "12345"), 0);

            sypha_opt_parse_free(opt_parse_result);
        }

        SUBCASE("Missing required param") {
            CHECK(sypha_opt_config_add_param(opt_config, "-z", "--sleep", 0, 1) != NULL);

            SYPHA_OPT_PARSE_RESULT opt_parse_result = sypha_opt_parse_args(opt_config, argc, argv);
            CHECK(opt_parse_result == NULL);
        }

        sypha_opt_config_free(opt_config);
    }

    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
}
