#ifndef ASH_STEPTIMER_H
#define ASH_STEPTIMER_H

#include <chrono>

namespace ash
{

    class StepTimerSimpleMilliseconds
    {
    public:
        StepTimerSimpleMilliseconds();
        ~StepTimerSimpleMilliseconds() = default;

        std::chrono::milliseconds Start();
        std::chrono::milliseconds Time();

    private:
        std::chrono::milliseconds m_StartTime;
    };

} // namespace ash

#endif // !ASH_STEPTIMER_H
