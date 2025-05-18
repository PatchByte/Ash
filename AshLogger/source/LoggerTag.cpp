#include "AshLogger/LoggerTag.h"
#include <algorithm>

namespace ash
{

    bool LoggerTagFilter::DoFilter(LoggerShortTag Short)
    {
        bool hasBeenFoundInList = (std::find(this->filterTrigger.begin(), this->filterTrigger.end(), Short) != this->filterTrigger.end());

        switch (this->filterMode)
        {
        case LoggerTagFilterMode::IGNORE_WHEN_LOGGED:
            return (hasBeenFoundInList == false);
        case LoggerTagFilterMode::ONLY_LOG_WHEN:
            return hasBeenFoundInList;

        // And you know, the other stuff that should not be supposed to be the case /*_*/
        case LoggerTagFilterMode::INVALID:
        default:
            return true;
        }
    }

} // namespace ash

// Stub file, for compilation
