#ifndef ASHLOGGER_LOGGER_H
#define ASHLOGGER_LOGGER_H

#include <functional>
#include <iostream>

#include "Ash/Buffer.h"
#include "Ash/Class.h"
#include "fmt/format.h"

#include "LoggerPassage.h"
#include "LoggerTag.h"

namespace ash::logger
{
    /*
     * This only creates a prefix string, this does not format the actual message.
     * Example: [Error] [17:42:05] [Thread:10]
     */
    using LoggerPrefixFunctionDelegate = std::function<std::string(Tag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs)>;

    class Logger
    {
    public:
        Logger(std::string LoggerName, TagInitializer LoggerTags);
        ~Logger();

        virtual void SetPrefixFunction(LoggerPrefixFunctionDelegate PrefixFunction);

        virtual std::string GetLoggerName();
        virtual LoggerPrefixFunctionDelegate GetPrefixFunction();

        virtual void AddLoggerPassage(Passage* LoggerPassage);

        inline bool Log(ShortTag LoggerTag, std::string LoggerFormat, const auto&... LoggerFormatArgs)
        {
            return this->LogWithFormatArguments(LoggerTag, LoggerFormat, fmt::make_format_args(LoggerFormatArgs...));
        }

        inline bool Log(std::string Tag, std::string LoggerFormat, const auto&... LoggerFormatArgs)
        {
            return this->LogWithFormatArgumentsStringTag(Tag, LoggerFormat, fmt::make_format_args(LoggerFormatArgs...));
        }

        virtual bool LogWithFormatArguments(ShortTag LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs);
        virtual bool LogWithFormatArgumentsStringTag(std::string LoggerTag, std::string LoggerFormat, fmt::format_args LoggerFormatArgs);

        virtual TagRegistry GetTagRegistry();
        virtual ash::Size GetLongestTagLength();

    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshLogger);
    };

} // namespace ash

#endif // !ASHLOGGER_LOGGER_H
