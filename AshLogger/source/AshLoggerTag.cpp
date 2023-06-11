#include "AshLogger/AshLoggerTag.h"
#include <algorithm>

namespace ash
{

    bool AshLoggingTagFilter::DoFilter(AshLoggingShortTag Short)
    {
        bool hasBeenFoundInList = (std::find(this->filterTrigger.begin(), this->filterTrigger.end(), Short) != this->filterTrigger.end());

        switch (this->filterMode)
        {
        case AshLoggingTagFilterMode::IGNORE_WHEN_LOGGED: return (hasBeenFoundInList == false);
        case AshLoggingTagFilterMode::ONLY_LOG_WHEN: return hasBeenFoundInList;

        // And you know, the other stuff that should not be supposed to be the case /*_*/
        case AshLoggingTagFilterMode::INVALID:
        default:
            return true;
        }
    }

}

// Stub file, for compilation