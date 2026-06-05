/*!
 * @file _i2cNoteDrain_test.cpp
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
#include <fff.h>

#include "n_lib.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(const char *, _noteI2CReceive, uint16_t, uint8_t *, uint16_t,
                uint32_t *)
FAKE_VOID_FUNC(NoteLockI2C)
FAKE_VOID_FUNC(NoteUnlockI2C)

namespace
{

size_t i2cLockDepth = 0;
bool i2cReceiveSawLock = false;

void lockI2C()
{
    ++i2cLockDepth;
}

void unlockI2C()
{
    if (i2cLockDepth > 0) {
        --i2cLockDepth;
    }
}

const char *i2cReceiveDrainedQueuedData(uint16_t, uint8_t *, uint16_t size,
                                        uint32_t *available)
{
    i2cReceiveSawLock = (i2cLockDepth > 0);
    if (size == 0) {
        *available = 3;
    } else {
        *available = 0;
    }
    return NULL;
}

SCENARIO("_i2cNoteDrain")
{
    SECTION("drains queued data under the I2C lock") {
        _noteI2CReceive_fake.custom_fake = i2cReceiveDrainedQueuedData;
        NoteLockI2C_fake.custom_fake = lockI2C;
        NoteUnlockI2C_fake.custom_fake = unlockI2C;

        _i2cNoteDrain();

        CHECK(NoteLockI2C_fake.call_count == 1);
        CHECK(NoteUnlockI2C_fake.call_count == 1);
        CHECK(_noteI2CReceive_fake.call_count == 2);
        CHECK(i2cReceiveSawLock);
        CHECK(i2cLockDepth == 0);
        CHECK(_noteI2CReceive_fake.arg1_history[0] != NULL);
        CHECK(_noteI2CReceive_fake.arg2_history[0] == 0);
        CHECK(_noteI2CReceive_fake.arg2_history[1] == 3);
    }

    SECTION("unlocks when receive returns an error") {
        _noteI2CReceive_fake.return_val = "some error";

        _i2cNoteDrain();

        CHECK(NoteLockI2C_fake.call_count == 1);
        CHECK(NoteUnlockI2C_fake.call_count == 1);
        CHECK(_noteI2CReceive_fake.call_count == 1);
    }

    RESET_FAKE(_noteI2CReceive);
    RESET_FAKE(NoteLockI2C);
    RESET_FAKE(NoteUnlockI2C);
    i2cLockDepth = 0;
    i2cReceiveSawLock = false;
}

}
