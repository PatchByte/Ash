#include "AshLogger/AshLoggerTag.h"
#include <algorithm>

namespace ash
{

    bool AshLoggerTagFilter::DoFilter(AshLoggerShortTag Short)
    {
        bool hasBeenFoundInList = (std::find(this->filterTrigger.begin(), this->filterTrigger.end(), Short) != this->filterTrigger.end());

        switch (this->filterMode)
        {
        case AshLoggerTagFilterMode::IGNORE_WHEN_LOGGED: return (hasBeenFoundInList == false);
        case AshLoggerTagFilterMode::ONLY_LOG_WHEN: return hasBeenFoundInList;

        // And you know, the other stuff that should not be supposed to be the case /*_*/
        case AshLoggerTagFilterMode::INVALID:
        default:
            return true;
        }
    }

}

// Stub file, for compilation