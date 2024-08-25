#include "doctest.h"
#include "syphac/sypha_env.h"
#include <string.h>
#include <stdlib.h>

// Note: these tests won't work on Windows but will on WSL

TEST_CASE("Test .env load") {

    // This isn't great, may need a better strategy
    system("ln -fs test/data/test_0.env .env");

    SUBCASE("Happy path value checks") {
        sypha_env_load_dot_env();

        const char * foo = sypha_env_get("foo");
        const char * bar = sypha_env_get("bar");
        const char * baz = sypha_env_get("baz");
        const char * fubar = sypha_env_get("fubar");

        REQUIRE(foo != NULL);
        CHECK_EQ(strcmp(foo, "bar"), 0);

        REQUIRE(bar != NULL);
        CHECK_EQ(strcmp(bar, "baz"), 0);

        REQUIRE(baz != NULL);
        CHECK_EQ(strcmp(baz, "1234567890abcdefghijklmnopqrstuvwxyz"), 0);

        CHECK(fubar == NULL);
    }

    // This isn't great, may need a better strategy
    system("rm -f .env");
}
