#ifndef ASHLOGGER_LOGGERTAG_H
#define ASHLOGGER_LOGGERTAG_H

#include <functional>
#include <iostream>
#include <vector>
#include <map>
#include <string>

namespace ash
{

    using LoggerShortTag = unsigned int;
    using LoggerShortTagPrefix = std::string;

    enum LoggerTagFilterMode : char
    {
        INVALID = 0,
        IGNORE_WHEN_LOGGED = 1,
        ONLY_LOG_WHEN = 2
    };

    class AshLoggerTag
    {
    public:
        inline AshLoggerTag(LoggerShortTag ShortTag, LoggerShortTagPrefix Prefix): shortTag(ShortTag), prefix(Prefix) {}
        inline AshLoggerTag(LoggerShortTagPrefix Prefix): prefix(Prefix)
        {
            shortTag = sfCreateShortTagByName(Prefix);
        }

        inline LoggerShortTag GetShortTag() { return shortTag; }
        inline LoggerShortTagPrefix GetPrefix() { return prefix; }

        static inline LoggerShortTag sfCreateShortTagByName(std::string Name)
        {
            auto nameHash = static_cast<LoggerShortTag>(std::hash<std::string>()(Name));
            return (nameHash & 0xFFFFFFFF) ^ (nameHash >> 31);
        }
    private:
        LoggerShortTag shortTag;
        LoggerShortTagPrefix prefix;
    };

    using LoggerTagInitializer = std::vector<AshLoggerTag>;
    using LoggerTagMap = std::map<LoggerShortTag, AshLoggerTag>;

    class LoggerTagRegistry
    {
    public:
        inline LoggerTagRegistry() = default;
        inline ~LoggerTagRegistry() = default;

        inline bool AddTag(AshLoggerTag Tag)
        {
            if(HasTag(Tag.GetShortTag()) == true)
            {
                return false;
            }

            tagMap.emplace(Tag.GetShortTag(), Tag);
            return true;
        }
        bool HasTag(LoggerShortTag ShortTag) { return tagMap.count(ShortTag) > 0; }
        AshLoggerTag GetTag(LoggerShortTag ShortTag)
        {
            if(HasTag(ShortTag) == false)
            {
                return AshLoggerTag(-1, "InvalidTag");
            }

            return tagMap.at(ShortTag);
        }
        inline LoggerTagMap GetTagMap() { return tagMap; }
    private:
        LoggerTagMap tagMap;
    };

    // @todo Change LoggerTag Filter to virtual.
    class LoggerTagFilter
    {
    public:
        inline LoggerTagFilter(LoggerTagFilterMode FilterMode, std::vector<LoggerShortTag> FilterTrigger): filterMode(FilterMode), filterTrigger(FilterTrigger) {}
        inline LoggerTagFilter(): filterMode(LoggerTagFilterMode::INVALID), filterTrigger() {}

        bool DoFilter(LoggerShortTag Short);
    private:
        LoggerTagFilterMode filterMode;
        std::vector<LoggerShortTag> filterTrigger;
    };
}

#endif // !ASHLOGGER_LOGGERTAG_H
