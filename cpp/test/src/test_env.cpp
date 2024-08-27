/* test_env.cpp
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
#include "syphacpp/sypha_env.hpp"
#include <stdlib.h>

// Note: these tests won't work on Windows but will on WSL

using namespace sypha;

TEST_CASE("Test .env load") {

    // This isn't great, may need a better strategy
    system("ln -fs test/data/test_0.env .env");

    Env e = Env();

    SUBCASE("Happy path file contents checks") {
        std::string foo;
        CHECK(e.get("foo", foo));
        CHECK_EQ(foo.compare("bar"), 0);

        std::string bar;
        CHECK(e.get("bar", bar));
        CHECK_EQ(bar.compare("baz"), 0);

        std::string baz;
        CHECK(e.get("baz", baz));
        CHECK_EQ(baz.compare("1234567890abcdefghijklmnopqrstuvwxyz"), 0);
    }

    SUBCASE("Missing value") {
        std::string fubar;

        CHECK(!e.get("fubar", fubar));
        
        // now add it then read it back
        e.set("fubar", "it works", 0);
        CHECK(e.get("fubar", fubar));
        CHECK_EQ(fubar.compare("it works"), 0);

        // don't overwrite
        e.set("fubar", "won't work", 0);
        CHECK(e.get("fubar", fubar));
        CHECK_EQ(fubar.compare("it works"), 0);

        // overwrite
        e.set("fubar", "it will work", 1);
        CHECK(e.get("fubar", fubar));
        CHECK_EQ(fubar.compare("it will work"), 0);
    }

    // This isn't great, may need a better strategy
    system("rm -f .env");
}

// TODO: add some more devious cases later
