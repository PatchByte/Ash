#ifndef ASHLOGGER_LOGGERTAG_H
#define ASHLOGGER_LOGGERTAG_H

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace ash::logger
{

    using ShortTag = unsigned int;
    using ShortTagPrefix = std::string;

    enum TagFilterMode : char
    {
        INVALID = 0,
        IGNORE_WHEN_LOGGED = 1,
        ONLY_LOG_WHEN = 2
    };

    class Tag
    {
    public:
        inline Tag(ShortTag ShortTag, ShortTagPrefix Prefix) : shortTag(ShortTag), prefix(Prefix)
        {
        }

        inline Tag(ShortTagPrefix Prefix) : prefix(Prefix)
        {
            shortTag = sfCreateShortTagByName(Prefix);
        }

        inline ShortTag GetShortTag()
        {
            return shortTag;
        }

        inline ShortTagPrefix GetPrefix()
        {
            return prefix;
        }

        static inline ShortTag sfCreateShortTagByName(std::string Name)
        {
            auto nameHash = static_cast<ShortTag>(std::hash<std::string>()(Name));
            return (nameHash & 0xFFFFFFFF) ^ (nameHash >> 31);
        }

    private:
        ShortTag shortTag;
        ShortTagPrefix prefix;
    };

    using TagInitializer = std::vector<Tag>;
    using TagMap = std::map<ShortTag, Tag>;

    class TagRegistry
    {
    public:
        inline TagRegistry() = default;
        inline ~TagRegistry() = default;

        inline bool AddTag(Tag Tag)
        {
            if (HasTag(Tag.GetShortTag()) == true)
            {
                return false;
            }

            m_TagMap.emplace(Tag.GetShortTag(), Tag);
            return true;
        }

        bool HasTag(ShortTag ShortTag)
        {
            return m_TagMap.count(ShortTag) > 0;
        }

        Tag GetTag(ShortTag ShortTag)
        {
            if (HasTag(ShortTag) == false)
            {
                return Tag(-1, "InvalidTag");
            }

            return m_TagMap.at(ShortTag);
        }

        inline TagMap GetTagMap()
        {
            return m_TagMap;
        }

    private:
        TagMap m_TagMap;
    };

    // @todo Change LoggerTag Filter to virtual.
    class TagFilter
    {
    public:
        inline TagFilter(TagFilterMode FilterMode, std::vector<ShortTag> FilterTrigger) : m_FilterMode(FilterMode), m_FilterTrigger(FilterTrigger)
        {
        }

        inline TagFilter() : m_FilterMode(TagFilterMode::INVALID), m_FilterTrigger()
        {
        }

        bool DoFilter(ShortTag Short);

    private:
        TagFilterMode m_FilterMode;
        std::vector<ShortTag> m_FilterTrigger;
    };
} // namespace ash

#endif // !ASHLOGGER_LOGGERTAG_H
