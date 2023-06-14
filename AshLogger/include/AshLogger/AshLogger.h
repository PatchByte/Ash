#pragma once
#include <Ash/AshClass.h>
#include <fmt/format.h>

#include <iostream>
#include <functional>

#include "AshLoggerTag.h"
#include "AshLoggerPassage.h"

namespace ash
{
    /*
     * This only creates a prefix string, this does not format the actual message.
     * Example: [Error] [17:42:05] [Thread:10]
     */
    using AshLoggerPrefixFunctionDelegate = std::function<std::string(AshLoggerTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)>;

    class AshLogger
    {
    public:
        AshLogger(std::string LoggerName, AshLoggerTagInitializer LoggerTags);
        ~AshLogger();

        virtual void SetPrefixFunction(AshLoggerPrefixFunctionDelegate PrefixFunction);
        virtual AshLoggerPrefixFunctionDelegate GetPrefixFunction();

        virtual void AddLoggerPassage(AshLoggerPassage* LoggerPassage);

        inline bool Log(AshLoggerShortTag LoggerTag, std::string LoggerFormat, const auto&... LoggerFormatArgs)
        {
            return this->LogWithFormatArguments(LoggerTag, LoggerFormat, fmt::make_format_args(LoggerFormatArgs...));
        }
        
        virtual bool LogWithFormatArguments(AshLoggerShortTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs);
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshLogger);
    };

}