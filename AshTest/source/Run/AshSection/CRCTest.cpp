#include "CRCTest.h"
#include <Ash/AshCRC32.h>
#include <iostream>

static constexpr std::string_view smCRCTestString = "Hello, World!";
static constexpr ash::AshCRC32Value smCRCExpectedValue = 0xec4ac3d0;

ash::AshResult CRC32RunTest::Do()
{
    ash::AshCRC32 crc = ash::AshCRC32();

    crc.InitAndClear();
    crc.Update((void*)(smCRCTestString.data()), smCRCTestString.length());

    if(crc.GetValue() != smCRCExpectedValue)
    {
        return ash::AshResult(false, "CRC32 value doesn't match with the expected CRC32 value.");
    }

    return ash::AshResult(true);
}