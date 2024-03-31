#include "Ash/AshStepTimer.h"
#include <chrono>

namespace ash
{

    AshStepTimerSimpleMilliseconds::AshStepTimerSimpleMilliseconds()
    {
        Start();
    }

    std::chrono::milliseconds AshStepTimerSimpleMilliseconds::Start()
    {
        startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return startTime;
    }

    std::chrono::milliseconds AshStepTimerSimpleMilliseconds::Time()
    {
        std::chrono::milliseconds v = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - startTime;
        startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return v;
    }

}