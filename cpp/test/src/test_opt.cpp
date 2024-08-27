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

#include "doctest.h"
#include <string.h>
#include "syphacpp/sypha_opt.hpp"

#define ARG_MAX_LEN     64
#define ARG_COUNT_MAX   16

using namespace sypha;

TEST_CASE("Happy Path CLI") {

    char * argv[ARG_COUNT_MAX];

    for (int i=0;i < ARG_COUNT_MAX; i++) {
        argv[i] = new char[ARG_MAX_LEN];
        *(argv[i]) = '\0';
    }

    SUBCASE("Happy Path Config") {
        strcpy(argv[0], "my_program");
        strcpy(argv[1], "-h");
        strcpy(argv[2], "localhost");
        strcpy(argv[3], "--port");
        strcpy(argv[4], "12345");
        int argc = 5;

        Opt::ParamSet paramSet;
        paramSet.insert(Opt::Param("-f", "--force", true, false));
        paramSet.insert(Opt::Param("-h", "--host", false, true));
        paramSet.insert(Opt::Param("-p", "--port", false, true));

        SUBCASE("Happy Path Result") {
            Opt opt(paramSet, argc, argv);

            std::string host;
            opt.get("--host", host);
            CHECK_EQ(host.compare("localhost"), 0);

            std::string port;
            opt.get("-p", port);
            CHECK_EQ(port.compare("12345"), 0);
        }

        SUBCASE("Missing required param") {
            paramSet.insert(Opt::Param("-z", "--sleep", false, true));

            bool threwUp = false;
            try {
                Opt opt(paramSet, argc, argv);
            } catch (std::exception e) {
                threwUp = true;
            }
            CHECK(threwUp);
        }

        SUBCASE("Flag") {
            strcpy(argv[5], "--force");
            argc = 6;

            Opt opt(paramSet, argc, argv);

            // Pass if not blow up
        }
    }

    for (int i = 0; i < ARG_COUNT_MAX; i++) {
        delete[] argv[i];
    }
}
