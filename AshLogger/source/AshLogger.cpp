#include "AshLogger/AshLogger.h"

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

        std::string loggerName;
        AshLoggerTagRegistry loggerTagRegistry;
        AshLoggerPrefixFunctionDelegate loggerPrefixFunction;
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
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshLogger);
    }

}