#include "AshLogger/Logger.h"
#include "Ash/Buffer.h"
#include "AshLogger/LoggerTag.h"
#include "fmt/base.h"

namespace ash::logger
{

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshLogger)
    {
    public:
        ClassAshLoggerInternalObject()
            : m_Name("Invalid"), m_TagRegistry({}),
              m_PrefixFunction([](Tag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs) -> std::string { return fmt::format("[{}]", LoggerTag.GetPrefix()); })
        {
        }

        ~ClassAshLoggerInternalObject() = default;

        std::string m_Name;
        TagRegistry m_TagRegistry;
        LoggerPrefixFunctionDelegate m_PrefixFunction;
        std::vector<Passage*> m_Passages;
    };

    Logger::Logger(std::string LoggerName, TagInitializer LoggerTags)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshLogger);

        classInternalAshLogger->m_Name = LoggerName;

        for (Tag c : LoggerTags)
        {
            classInternalAshLogger->m_TagRegistry.AddTag(c);
        }
    }

    Logger::~Logger()
    {
        for (Passage* c : classInternalAshLogger->m_Passages)
        {
            delete c;
        }

        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshLogger);
    }

    void Logger::SetPrefixFunction(LoggerPrefixFunctionDelegate PrefixFunction)
    {
        classInternalAshLogger->m_PrefixFunction = PrefixFunction;
    }

    std::string Logger::GetLoggerName()
    {
        return classInternalAshLogger->m_Name;
    }

    LoggerPrefixFunctionDelegate Logger::GetPrefixFunction()
    {
        return classInternalAshLogger->m_PrefixFunction;
    }

    void Logger::AddLoggerPassage(Passage* LoggerPassage)
    {
        LoggerPassage->SetParent(this, this->classInternalAshLogger->m_Name);
        this->classInternalAshLogger->m_Passages.push_back(LoggerPassage);
    }

    bool Logger::LogWithFormatArguments(ShortTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)
    {
        if (this->classInternalAshLogger->m_TagRegistry.HasTag(LoggerTag) == false)
        {
            // @todo Implement a sort of "Backup LoggerTag"
            return false;
        }

        Tag resolvedLoggerTag = this->classInternalAshLogger->m_TagRegistry.GetTag(LoggerTag);
        std::string formattedString = fmt::vformat(LoggerFormat, LoggerFormatArgs);

        for (auto currentPassage : classInternalAshLogger->m_Passages)
        {
            currentPassage->DoPassthrough(resolvedLoggerTag, LoggerFormat, LoggerFormatArgs, formattedString);
        }

        return true;
    }

    bool Logger::LogWithFormatArgumentsStringTag(std::string LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)
    {
        ShortTag shortTag = Tag::sfCreateShortTagByName(LoggerTag);

        if (classInternalAshLogger->m_TagRegistry.HasTag(shortTag) == false)
        {
            classInternalAshLogger->m_TagRegistry.AddTag(Tag(LoggerTag));
        }

        return this->LogWithFormatArguments(shortTag, LoggerFormat, LoggerFormatArgs);
    }

    TagRegistry Logger::GetTagRegistry()
    {
        return classInternalAshLogger->m_TagRegistry;
    }

    ash::Size Logger::GetLongestTagLength()
    {
        ash::Size maxLength = 0;

        for (auto currentTag : classInternalAshLogger->m_TagRegistry.GetTagMap())
        {
            maxLength = maxLength > currentTag.second.GetPrefix().length() ? maxLength : currentTag.second.GetPrefix().length();
        }

        return 0;
    }

} // namespace ash
