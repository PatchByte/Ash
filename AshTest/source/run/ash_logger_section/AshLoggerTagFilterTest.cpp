#include "AshLoggerTagFilterTest.h"
#include <iostream>

#include <Ash/AshCRC32.h>
#include <AshLogger/AshLoggerTag.h>

#define LOG_TAG_TEST_ONE 0xdead
#define LOG_TAG_TEST_TWO 0xbeef
#define LOG_TAG_TEST_THREE 0xfeed
#define LOG_TAG_TEST_FOUR 0xc0de

static std::vector<ash::AshLoggerShortTag> smAshLoggerTestTags = {
    LOG_TAG_TEST_ONE,
    LOG_TAG_TEST_TWO,
    LOG_TAG_TEST_THREE,
    LOG_TAG_TEST_FOUR
};

static constexpr ash::CRC32 smAshLoggerFilterTestExpectedCRC = 0x17ab50ab;

ash::AshResult AshLoggerTagFilterTest::Do()
{
    ash::AshCRC32 testCRC = ash::AshCRC32();

    for(ash::AshLoggerTagFilterMode currentMode : std::vector<ash::AshLoggerTagFilterMode>({ ash::AshLoggerTagFilterMode::ONLY_LOG_WHEN, ash::AshLoggerTagFilterMode::IGNORE_WHEN_LOGGED }))
    {
        ash::AshLoggerTagFilter ashLoggerTagFilterTest = ash::AshLoggerTagFilter(currentMode, {
            LOG_TAG_TEST_ONE,
            LOG_TAG_TEST_FOUR
        });

        for(ash::AshLoggerShortTag currentTag : smAshLoggerTestTags)
        {
            bool result = ashLoggerTagFilterTest.DoFilter(currentTag);

            testCRC.UpdateWithTemplateValue(currentTag);
            testCRC.UpdateWithTemplateValue(currentTag * result);
        }
    }

    if(testCRC.GetValue() != smAshLoggerFilterTestExpectedCRC)
    {
        return ash::AshResult(false, "Expected test crc value did not match with the actual crc value.");
    }

    return ash::AshResult(true);
}