#include "Ash/StepTimer.h"
#include <chrono>

namespace ash
{

    StepTimerSimpleMilliseconds::StepTimerSimpleMilliseconds()
    {
        Start();
    }

    std::chrono::milliseconds StepTimerSimpleMilliseconds::Start()
    {
        m_StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return m_StartTime;
    }

    std::chrono::milliseconds StepTimerSimpleMilliseconds::Time()
    {
        std::chrono::milliseconds v = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - m_StartTime;
        m_StartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return v;
    }

} // namespace ash
