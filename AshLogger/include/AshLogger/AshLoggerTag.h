#pragma once
#include <iostream>
#include <vector>

namespace ash
{

    using AshLoggingShortTag = unsigned int;
    using AshLoggingShortTagPrefix = std::string_view;
    
    enum AshLoggingTagFilterMode : char
    {
        INVALID = 0,
        IGNORE_WHEN_LOGGED = 1,
        ONLY_LOG_WHEN = 2
    };

    class AshLoggingTag
    {
    public:
        inline AshLoggingTag(AshLoggingShortTag ShortTag, AshLoggingShortTagPrefix Prefix): shortTag(ShortTag), prefix(Prefix) {}

        inline AshLoggingShortTag GetShortTag() { return shortTag; }
        inline AshLoggingShortTagPrefix GetPrefix() { return prefix; }
    private:
        AshLoggingShortTag shortTag; 
        AshLoggingShortTagPrefix prefix;
    };

    class AshLoggingTagFilter
    {
    public:
        inline AshLoggingTagFilter(AshLoggingTagFilterMode FilterMode, std::vector<AshLoggingShortTag> FilterTrigger): filterMode(FilterMode), filterTrigger(FilterTrigger) {}

        bool DoFilter(AshLoggingShortTag Short);
    private:
        AshLoggingTagFilterMode filterMode;
        std::vector<AshLoggingShortTag> filterTrigger;
    };
}