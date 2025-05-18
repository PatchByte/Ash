#include "CRCTest.h"
#include <Ash/AshCRC32.h>
#include <iostream>

static constexpr std::string_view smCRCTestString = "Hello, World!";
static constexpr ash::CRC32 smCRCExpectedValue = 0xec4ac3d0;

ash::Result CRC32RunTest::Do()
{
    ash::CRC32State crc = ash::CRC32();

    crc.InitAndClear();
    crc.Update((void*)(smCRCTestString.data()), smCRCTestString.length());

    if(crc.GetValue() != smCRCExpectedValue)
    {
        return ash::Result(false, "CRC32 value doesn't match with the expected CRC32 value.");
    }

    return ash::Result(true);
}
