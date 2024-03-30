#include "Ash/AshStepTimer.h"
#include <chrono>

namespace ash
{

    AshStepTimerSimpleMiliseconds::AshStepTimerSimpleMiliseconds()
    {
        Start();
    }

    std::chrono::milliseconds AshStepTimerSimpleMiliseconds::Start()
    {
        startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return startTime;
    }

    std::chrono::milliseconds AshStepTimerSimpleMiliseconds::Time()
    {
        std::chrono::milliseconds v = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - startTime;
        startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return v;
    }

}