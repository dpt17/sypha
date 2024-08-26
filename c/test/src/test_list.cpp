/* test_list.cpp
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
#include "syphac/sypha_list.h"
#include <string.h>

TEST_CASE("Happy Path List") {
    SYPHA_LIST list = sypha_list_create();
    REQUIRE(list != NULL);

    SUBCASE("Create list with Append") {
        const char * token;

        token = "foo";
        sypha_list_append_item(list, (void *) token, strlen(token) + 1);

        token = "bar";
        sypha_list_append_item(list, (void *) token, strlen(token) + 1);

        token = "fubar";
        sypha_list_append_item(list, (void *) token, strlen(token) + 1);

        SUBCASE("Forward iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }

        SUBCASE("Reverse iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }
    }

    SUBCASE("Create list with Prepend") {
        const char * token;

        token = "fubar";
        sypha_list_prepend_item(list, (void *) token, strlen(token) + 1);

        token = "bar";
        sypha_list_prepend_item(list, (void *) token, strlen(token) + 1);

        token = "foo";
        sypha_list_prepend_item(list, (void *) token, strlen(token) + 1);

        SUBCASE("Forward iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }

        SUBCASE("Reverse iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }
    }

    SUBCASE("Create list with both") {
        const char * token;

        token = "bar";
        sypha_list_append_item(list, (void *) token, strlen(token) + 1);

        token = "foo";
        sypha_list_prepend_item(list, (void *) token, strlen(token) + 1);

        token = "fubar";
        sypha_list_append_item(list, (void *) token, strlen(token) + 1);

        SUBCASE("Forward iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }

        SUBCASE("Reverse iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }
    }

    sypha_list_destroy(list);
}

TEST_CASE("Modify list via iterator") {
    SYPHA_LIST list = sypha_list_create();
    REQUIRE(list != NULL);

    SUBCASE("Insert after with forward iterator") {
        SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
        REQUIRE(iterator != NULL);

        const char * token;

        token = "fubar";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        token = "bar";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        token = "foo";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        sypha_list_destroy_iterator(iterator);

        SUBCASE("Forward iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }

        SUBCASE("Reverse iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }
    }

    SUBCASE("Insert after with backwards iterator") {
        SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
        REQUIRE(iterator != NULL);

        const char * token;

        token = "foo";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        token = "bar";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        token = "fubar";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        sypha_list_destroy_iterator(iterator);

        SUBCASE("Forward iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }

        SUBCASE("Reverse iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }
    }

    SUBCASE("Mixed insert with forward iterator") {
        SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
        REQUIRE(iterator != NULL);

        const char * token;

        token = "fubar";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        token = "bar";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        CHECK_EQ(sypha_list_iterator_next(iterator), 0);

        token = "foo";
        CHECK_EQ(sypha_list_iterator_insert_before(iterator, (void *) token, strlen(token) + 1), 0);

        sypha_list_destroy_iterator(iterator);

        SUBCASE("Forward iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }

        SUBCASE("Reverse iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }
    }

    SUBCASE("Mixed insert with backward iterator") {
        SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
        REQUIRE(iterator != NULL);

        const char * token;

        token = "foo";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        token = "bar";
        CHECK_EQ(sypha_list_iterator_insert_after(iterator, (void *) token, strlen(token) + 1), 0);

        CHECK_EQ(sypha_list_iterator_next(iterator), 0);

        token = "fubar";
        CHECK_EQ(sypha_list_iterator_insert_before(iterator, (void *) token, strlen(token) + 1), 0);

        sypha_list_destroy_iterator(iterator);

        SUBCASE("Forward iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_front(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }

        SUBCASE("Reverse iterator") {
            SYPHA_LIST_ITERATOR iterator = sypha_list_get_iterator_back(list);
            REQUIRE(iterator != NULL);

            void * value;
            size_t valueSz;

            // Move through list
            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_next(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "foo"), 0);

            CHECK_LT(sypha_list_iterator_next(iterator), 0);

            // Go back to front
            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "bar"), 0);

            CHECK_EQ(sypha_list_iterator_previous(iterator), 0);
            value = sypha_list_iterator_get(iterator, &valueSz);
            CHECK_EQ(strcmp((const char *) value, "fubar"), 0);

            CHECK_LT(sypha_list_iterator_previous(iterator), 0);

            sypha_list_destroy_iterator(iterator);
        }
    }

    sypha_list_destroy(list);
}
