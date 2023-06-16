#include "AshLoggerTest.h"
#if ASH_ENABLE_LOGGER
#include <AshLogger/AshLogger.h>

ash::AshResult AshLoggerTest::Do()
{
    ash::AshLogger logger = ash::AshLogger("TestLogger", ash::AshLoggerTagInitializer({
        ash::AshLoggerTag(0x1, "Info")
    }));

    return ash::AshResult(true);
}

#endif