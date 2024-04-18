#pragma once

#include <chrono>
namespace ash
{

    class AshStepTimerSimpleMilliseconds
    {
    public:
        AshStepTimerSimpleMilliseconds();
        ~AshStepTimerSimpleMilliseconds() = default;

        std::chrono::milliseconds Start();
        std::chrono::milliseconds Time();
    private:
        std::chrono::milliseconds m_StartTime;
    };

}
