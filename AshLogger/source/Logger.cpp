#include "AshLogger/Logger.h"
#include "Ash/Buffer.h"
#include "AshLogger/LoggerTag.h"
#include "fmt/base.h"

namespace ash
{

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshLogger)
    {
    public:
        ClassAshLoggerInternalObject()
            : loggerName("Invalid"), loggerTagRegistry({}),
              loggerPrefixFunction([](AshLoggerTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs) -> std::string { return fmt::format("[{}]", LoggerTag.GetPrefix()); })
        {
        }

        ~ClassAshLoggerInternalObject() = default;

        std::string loggerName;
        LoggerTagRegistry loggerTagRegistry;
        LoggerPrefixFunctionDelegate loggerPrefixFunction;
        std::vector<LoggerPassage*> loggerPassages;
    };

    Logger::Logger(std::string LoggerName, LoggerTagInitializer LoggerTags)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshLogger);

        classInternalAshLogger->loggerName = LoggerName;

        for (AshLoggerTag c : LoggerTags)
        {
            classInternalAshLogger->loggerTagRegistry.AddTag(c);
        }
    }

    Logger::~Logger()
    {
        for (LoggerPassage* c : classInternalAshLogger->loggerPassages)
        {
            delete c;
        }

        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshLogger);
    }

    void Logger::SetPrefixFunction(LoggerPrefixFunctionDelegate PrefixFunction)
    {
        classInternalAshLogger->loggerPrefixFunction = PrefixFunction;
    }

    std::string Logger::GetLoggerName()
    {
        return classInternalAshLogger->loggerName;
    }

    LoggerPrefixFunctionDelegate Logger::GetPrefixFunction()
    {
        return classInternalAshLogger->loggerPrefixFunction;
    }

    void Logger::AddLoggerPassage(LoggerPassage* LoggerPassage)
    {
        LoggerPassage->SetParent(this, this->classInternalAshLogger->loggerName);
        this->classInternalAshLogger->loggerPassages.push_back(LoggerPassage);
    }

    bool Logger::LogWithFormatArguments(LoggerShortTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)
    {
        if (this->classInternalAshLogger->loggerTagRegistry.HasTag(LoggerTag) == false)
        {
            // @todo Implement a sort of "Backup LoggerTag"
            return false;
        }

        AshLoggerTag resolvedLoggerTag = this->classInternalAshLogger->loggerTagRegistry.GetTag(LoggerTag);
        std::string formattedString = fmt::vformat(LoggerFormat, LoggerFormatArgs);

        for (auto currentPassage : classInternalAshLogger->loggerPassages)
        {
            currentPassage->DoPassthrough(resolvedLoggerTag, LoggerFormat, LoggerFormatArgs, formattedString);
        }

        return true;
    }

    bool Logger::LogWithFormatArgumentsStringTag(std::string LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)
    {
        LoggerShortTag shortTag = AshLoggerTag::sfCreateShortTagByName(LoggerTag);

        if (classInternalAshLogger->loggerTagRegistry.HasTag(shortTag) == false)
        {
            classInternalAshLogger->loggerTagRegistry.AddTag(AshLoggerTag(LoggerTag));
        }

        return this->LogWithFormatArguments(shortTag, LoggerFormat, LoggerFormatArgs);
    }

    ash::LoggerTagRegistry Logger::GetTagRegistry()
    {
        return classInternalAshLogger->loggerTagRegistry;
    }

    ash::Size Logger::GetLongestTagLength()
    {
        ash::Size maxLength = 0;

        for (auto currentTag : classInternalAshLogger->loggerTagRegistry.GetTagMap())
        {
            maxLength = maxLength > currentTag.second.GetPrefix().length() ? maxLength : currentTag.second.GetPrefix().length();
        }

        return 0;
    }

} // namespace ash
