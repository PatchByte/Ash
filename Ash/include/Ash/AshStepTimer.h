#pragma once

#include <chrono>
namespace ash
{

    class AshStepTimerSimpleMiliseconds
    {
    public:
        AshStepTimerSimpleMiliseconds();
        ~AshStepTimerSimpleMiliseconds() = default;

        std::chrono::milliseconds Start();
        std::chrono::milliseconds Time();
    private:
        std::chrono::milliseconds startTime;
    };

}