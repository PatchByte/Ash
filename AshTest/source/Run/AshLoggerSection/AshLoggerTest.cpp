#include "AshLoggerTest.h"
#if ASH_ENABLE_LOGGER
#include <AshLogger/AshLogger.h>
#include <Ash/AshCRC32.h>

#define ASH_TAG_INFO 0x1
#define ASH_TAG_WARNING 0x2
#define ASH_TAG_ERROR 0x3

static constexpr ash::AshCRC32Value smAshLoggerTestExpectedCRC = 0x29790cb;

ash::AshResult AshLoggerTest::Do()
{
    ash::AshCRC32 testCRC = ash::AshCRC32();

    ash::AshLogger* logger = new ash::AshLogger("TestLogger", ash::AshLoggerTagInitializer({
        ash::AshLoggerTag(ASH_TAG_INFO, "Info"),
        ash::AshLoggerTag(ASH_TAG_WARNING, "Warning"),
        ash::AshLoggerTag(ASH_TAG_ERROR, "Error")
    }));

    ash::AshLoggerPassage* testPassage = new ash::AshLoggerFunctionPassage([&testCRC] (ash::AshLoggerDefaultPassage* This, ash::AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString) -> void 
    {
        if(This->GetLoggerTagFilter().DoFilter(Tag.GetShortTag()) == false)
        {
            testCRC.UpdateWithTemplateValue<unsigned int>(0xdead);
            return;
        }

        std::string prefix = This->GetParent()->GetPrefixFunction()(Tag, Format, Args);
        std::string printOutString = fmt::format("{} {}", prefix, FormattedString);

        std::cout << printOutString << std::endl;

        testCRC.Update(printOutString.data(), printOutString.size());
    });

    testPassage->ApplyFilterForPassage(ash::AshLoggerTagFilter(ash::AshLoggerTagFilterMode::ONLY_LOG_WHEN, {
        ASH_TAG_INFO,
        ASH_TAG_ERROR
    }));

    logger->AddLoggerPassage(testPassage);

    logger->Log(ASH_TAG_INFO, "Hello {}", "World!");
    logger->Log(ASH_TAG_WARNING, "!Hello{}", "World!");
    logger->Log(ASH_TAG_ERROR, "{}", "World! Hello");

    testPassage->ApplyFilterForPassage(ash::AshLoggerTagFilter(ash::AshLoggerTagFilterMode::IGNORE_WHEN_LOGGED, {
        ASH_TAG_INFO,
        ASH_TAG_ERROR
    }));

    logger->Log(ASH_TAG_INFO, "Hello {}", "World!");
    logger->Log(ASH_TAG_WARNING, "!Hello{}", "World!");
    logger->Log(ASH_TAG_ERROR, "{}", "World! Hello");

    if(testCRC.GetValue() != smAshLoggerTestExpectedCRC)
    {
        return ash::AshResult(false, "Expected test crc value did not match with the actual crc value.");
    }

    return ash::AshResult(true);
}

#endif