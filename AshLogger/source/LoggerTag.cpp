#include "AshLogger/LoggerTag.h"
#include <algorithm>

namespace ash::logger
{

    bool TagFilter::DoFilter(ShortTag Short)
    {
        bool hasBeenFoundInList = (std::find(this->m_FilterTrigger.begin(), this->m_FilterTrigger.end(), Short) != this->m_FilterTrigger.end());

        switch (this->m_FilterMode)
        {
        case TagFilterMode::IGNORE_WHEN_LOGGED:
            return (hasBeenFoundInList == false);
        case TagFilterMode::ONLY_LOG_WHEN:
            return hasBeenFoundInList;

        // And you know, the other stuff that should not be supposed to be the case /*_*/
        case TagFilterMode::INVALID:
        default:
            return true;
        }
    }

} // namespace ash

// Stub file, for compilation
