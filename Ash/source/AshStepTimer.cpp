#include "Ash/AshStepTimer.h"
#include <chrono>
#include <pthread.h>

namespace ash
{

    AshStepTimerSimpleMilliseconds::AshStepTimerSimpleMilliseconds()
    {
        Start();
    }

    std::chrono::milliseconds AshStepTimerSimpleMilliseconds::Start()
    {
        m_StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return m_StartTime;
    }

    std::chrono::milliseconds AshStepTimerSimpleMilliseconds::Time()
    {
        std::chrono::milliseconds v = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - m_StartTime;
        m_StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return v;
    }

}
