#include "AshLogger/AshLogger.h"
#include "Ash/AshBuffer.h"
#include "AshLogger/AshLoggerTag.h"
#include "fmt/base.h"

namespace ash
{

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshLogger)
    {
    public:
        ClassAshLoggerInternalObject():
            loggerName("Invalid"),
            loggerTagRegistry({}),
            loggerPrefixFunction([] (AshLoggerTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs) -> std::string { return fmt::format("[{}]", LoggerTag.GetPrefix()); })
        {}

        ~ClassAshLoggerInternalObject() = default;

        std::string loggerName;
        AshLoggerTagRegistry loggerTagRegistry;
        AshLoggerPrefixFunctionDelegate loggerPrefixFunction;
        std::vector<AshLoggerPassage*> loggerPassages;
    };

    AshLogger::AshLogger(std::string LoggerName, AshLoggerTagInitializer LoggerTags)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshLogger);

        classInternalAshLogger->loggerName = LoggerName;

        for(AshLoggerTag c : LoggerTags)
        {
            classInternalAshLogger->loggerTagRegistry.AddTag(c);
        }
    }

    AshLogger::~AshLogger()
    {
        for(AshLoggerPassage* c : classInternalAshLogger->loggerPassages)
        {
            delete c;
        }

        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshLogger);
    }

    void AshLogger::SetPrefixFunction(AshLoggerPrefixFunctionDelegate PrefixFunction)
    {
        classInternalAshLogger->loggerPrefixFunction = PrefixFunction;
    }

    std::string AshLogger::GetLoggerName()
    {
        return classInternalAshLogger->loggerName;
    }

    AshLoggerPrefixFunctionDelegate AshLogger::GetPrefixFunction()
    {
        return classInternalAshLogger->loggerPrefixFunction;
    }

    void AshLogger::AddLoggerPassage(AshLoggerPassage* LoggerPassage)
    {
        LoggerPassage->SetParent(this, this->classInternalAshLogger->loggerName);
        this->classInternalAshLogger->loggerPassages.push_back(LoggerPassage);
    }

    bool AshLogger::LogWithFormatArguments(AshLoggerShortTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)
    {
        if(this->classInternalAshLogger->loggerTagRegistry.HasTag(LoggerTag) == false)
        {
            // @todo Implement a sort of "Backup LoggerTag"
            return false;
        }

        AshLoggerTag resolvedLoggerTag = this->classInternalAshLogger->loggerTagRegistry.GetTag(LoggerTag);
        std::string formattedString = fmt::vformat(LoggerFormat, LoggerFormatArgs);

        for(auto currentPassage : classInternalAshLogger->loggerPassages)
        {
            currentPassage->DoPassthrough(resolvedLoggerTag, LoggerFormat, LoggerFormatArgs, formattedString);
        }

        return true;
    }

    bool AshLogger::LogWithFormatArgumentsStringTag(std::string LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)
    {
        AshLoggerShortTag shortTag = AshLoggerTag::sfCreateShortTagByName(LoggerTag);

        if(classInternalAshLogger->loggerTagRegistry.HasTag(shortTag) == false)
        {
            classInternalAshLogger->loggerTagRegistry.AddTag(AshLoggerTag(LoggerTag));
        }

        return this->LogWithFormatArguments(shortTag, LoggerFormat, LoggerFormatArgs);
    }

    ash::AshLoggerTagRegistry AshLogger::GetTagRegistry()
    {
        return classInternalAshLogger->loggerTagRegistry;
    }

    ash::Size AshLogger::GetLongestTagLength()
    {
        ash::Size maxLength = 0;

        for(auto currentTag : classInternalAshLogger->loggerTagRegistry.GetTagMap())
        {
            maxLength = maxLength > currentTag.second.GetPrefix().length() ? maxLength : currentTag.second.GetPrefix().length();
        }

        return 0;
    }

}
