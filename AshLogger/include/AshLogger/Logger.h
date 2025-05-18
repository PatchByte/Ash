#pragma once

#include <iostream>
#include <functional>

#include "fmt/format.h"
#include "fmt/base.h"
#include "Ash/Class.h"
#include "Ash/Buffer.h"

#include "LoggerTag.h"
#include "LoggerPassage.h"

namespace ash
{
    /*
     * This only creates a prefix string, this does not format the actual message.
     * Example: [Error] [17:42:05] [Thread:10]
     */
    using LoggerPrefixFunctionDelegate = std::function<std::string(AshLoggerTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)>;

    class Logger
    {
    public:
        Logger(std::string LoggerName, LoggerTagInitializer LoggerTags);
        ~Logger();

        virtual void SetPrefixFunction(LoggerPrefixFunctionDelegate PrefixFunction);

        virtual std::string GetLoggerName();
        virtual LoggerPrefixFunctionDelegate GetPrefixFunction();

        virtual void AddLoggerPassage(LoggerPassage* LoggerPassage);

        inline bool Log(LoggerShortTag LoggerTag, std::string LoggerFormat, const auto&... LoggerFormatArgs)
        {
            return this->LogWithFormatArguments(LoggerTag, LoggerFormat, fmt::make_format_args(LoggerFormatArgs...));
        }

        inline bool Log(std::string Tag, std::string LoggerFormat, const auto&... LoggerFormatArgs)
        {
            return this->LogWithFormatArgumentsStringTag(Tag, LoggerFormat, fmt::make_format_args(LoggerFormatArgs...));
        }

        virtual bool LogWithFormatArguments(LoggerShortTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs);
        virtual bool LogWithFormatArgumentsStringTag(std::string LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs);

        virtual ash::LoggerTagRegistry GetTagRegistry();
        virtual ash::Size GetLongestTagLength();
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshLogger);
    };

}
