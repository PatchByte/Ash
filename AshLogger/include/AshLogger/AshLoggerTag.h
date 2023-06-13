#pragma once
#include <iostream>
#include <vector>
#include <map>

namespace ash
{

    using AshLoggerShortTag = unsigned int;
    using AshLoggerShortTagPrefix = std::string_view;
    
    enum AshLoggerTagFilterMode : char
    {
        INVALID = 0,
        IGNORE_WHEN_LOGGED = 1,
        ONLY_LOG_WHEN = 2
    };

    class AshLoggerTag
    {
    public:
        inline AshLoggerTag(AshLoggerShortTag ShortTag, AshLoggerShortTagPrefix Prefix): shortTag(ShortTag), prefix(Prefix) {}

        inline AshLoggerShortTag GetShortTag() { return shortTag; }
        inline AshLoggerShortTagPrefix GetPrefix() { return prefix; }
    private:
        AshLoggerShortTag shortTag; 
        AshLoggerShortTagPrefix prefix;
    };

    using AshLoggerTagInitializer = std::vector<AshLoggerTag>;
    using AshLoggerTagMap = std::map<AshLoggerShortTag, AshLoggerTag>;

    class AshLoggerTagFilter
    {
    public:
        inline AshLoggerTagFilter(AshLoggerTagFilterMode FilterMode, std::vector<AshLoggerShortTag> FilterTrigger): filterMode(FilterMode), filterTrigger(FilterTrigger) {}
        inline AshLoggerTagFilter(): filterMode(AshLoggerTagFilterMode::INVALID), filterTrigger() {}

        bool DoFilter(AshLoggerShortTag Short);
    private:
        AshLoggerTagFilterMode filterMode;
        std::vector<AshLoggerShortTag> filterTrigger;
    };
}