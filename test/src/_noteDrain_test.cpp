/*!
 * @file _noteDrain_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2026 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>

#include "n_lib.h"

typedef void (*nDrainFn) (void);

extern volatile int hookActiveInterface;
extern nDrainFn notecardDrain;

namespace
{

size_t hookCallCount = 0;

void drainHook(void)
{
    ++hookCallCount;
}

SCENARIO("_noteDrain")
{
    hookCallCount = 0;

    GIVEN("hookActiveInterface is set to NOTE_C_INTERFACE_NONE") {
        hookActiveInterface = NOTE_C_INTERFACE_NONE;

        AND_GIVEN("notecardDrain is unset (NULL)") {
            notecardDrain = NULL;

            WHEN("_noteDrain is called") {
                _noteDrain();

                THEN("no drain hook is called") {
                    CHECK(hookCallCount == 0);
                }
            }
        }

        AND_GIVEN("notecardDrain is set") {
            notecardDrain = drainHook;

            WHEN("_noteDrain is called") {
                _noteDrain();

                THEN("no drain hook is called") {
                    CHECK(hookCallCount == 0);
                }
            }
        }
    }

    GIVEN("hookActiveInterface is set to NOTE_C_INTERFACE_I2C") {
        hookActiveInterface = NOTE_C_INTERFACE_I2C;

        AND_GIVEN("notecardDrain is unset (NULL)") {
            notecardDrain = NULL;

            WHEN("_noteDrain is called") {
                _noteDrain();

                THEN("no drain hook is called") {
                    CHECK(hookCallCount == 0);
                }
            }
        }

        AND_GIVEN("notecardDrain is set") {
            notecardDrain = drainHook;

            WHEN("_noteDrain is called") {
                _noteDrain();

                THEN("it calls notecardDrain") {
                    CHECK(hookCallCount == 1);
                }
            }
        }
    }

    GIVEN("hookActiveInterface is set to NOTE_C_INTERFACE_SERIAL") {
        hookActiveInterface = NOTE_C_INTERFACE_SERIAL;

        AND_GIVEN("notecardDrain is unset (NULL)") {
            notecardDrain = NULL;

            WHEN("_noteDrain is called") {
                _noteDrain();

                THEN("no drain hook is called") {
                    CHECK(hookCallCount == 0);
                }
            }
        }

        AND_GIVEN("notecardDrain is set") {
            notecardDrain = drainHook;

            WHEN("_noteDrain is called") {
                _noteDrain();

                THEN("it calls notecardDrain") {
                    CHECK(hookCallCount == 1);
                }
            }
        }
    }
}

}
