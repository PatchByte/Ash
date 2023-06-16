#include "AshLoggerTest.h"
#if ASH_ENABLE_LOGGER
#include <AshLogger/AshLogger.h>
#include <Ash/AshCRC32.h>

#define ASH_TAG_INFO 0x1
#define ASH_TAG_ERROR 0x2

static constexpr ash::CRC32 smAshLoggerTestExpectedCRC = 0xcceac27;

ash::AshResult AshLoggerTest::Do()
{
    ash::AshLogger* logger = new ash::AshLogger("TestLogger", ash::AshLoggerTagInitializer({
        ash::AshLoggerTag(ASH_TAG_INFO, "Info"),
        ash::AshLoggerTag(ASH_TAG_ERROR, "Error")
    }));

    ash::AshCRC32 testCRC = ash::AshCRC32();

    logger->AddLoggerPassage(new ash::AshLoggerFunctionPassage([&testCRC] (ash::AshLoggerDefaultPassage* This, ash::AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString) -> void 
    {
        std::string prefix = This->GetParent()->GetPrefixFunction()(Tag, Format, Args);
        std::string printOutString = fmt::format("{} {}", prefix, FormattedString);

        testCRC.Update(printOutString.data(), printOutString.size());
    }));

    logger->Log(ASH_TAG_INFO, "Hello {}", "World!");
    logger->Log(ASH_TAG_ERROR, "{}", "World! Hello");

    if(testCRC.GetValue() != smAshLoggerTestExpectedCRC)
    {
        return ash::AshResult(false, "Expected test crc value did not match with the actual crc value.");
    }

    return ash::AshResult(true);
}

#endif