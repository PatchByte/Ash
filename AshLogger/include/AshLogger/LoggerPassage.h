#ifndef ASHLOGGER_LOGGERPASSAGE_H
#define ASHLOGGER_LOGGERPASSAGE_H

#include <iostream>
#include <functional>
#include <fmt/core.h>

#include "LoggerTag.h"

namespace ash
{

    class Logger;

    class LoggerPassage
    {
    public:
        virtual ~LoggerPassage() = default;
        virtual void SetParent(Logger* Parent, std::string ParentRegistrationName) {}
        virtual void DoPassthrough(AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString) {}
        virtual void ApplyFilterForPassage(LoggerTagFilter LoggerTagFilter) {}
    };

    class LoggerDefaultPassage : public LoggerPassage
    {
    public:
        LoggerDefaultPassage():
            parent(nullptr),
            parentRegistrationName(),
            loggerTagFilter()
        {}

        void SetParent(Logger* Parent, std::string ParentRegistrationName)
        {
            parent = Parent;
            parentRegistrationName = ParentRegistrationName;
        }

        void ApplyFilterForPassage(LoggerTagFilter LoggerTagFilter)
        {
            loggerTagFilter = LoggerTagFilter;
        }

        virtual Logger* GetParent() { return parent; }
        virtual std::string GetParentRegistrationName() {return parentRegistrationName; }
        virtual LoggerTagFilter GetLoggerTagFilter() { return loggerTagFilter; }
    private:
        Logger* parent;
        std::string parentRegistrationName;
        LoggerTagFilter loggerTagFilter;
    };

    class LoggerFunctionPassage : public LoggerDefaultPassage
    {
    public:
        using Delegate = std::function<void(LoggerDefaultPassage* This, AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString)>;

        LoggerFunctionPassage(Delegate DelegateFunction): delegateFunction(DelegateFunction) {}
        ~LoggerFunctionPassage() = default;

        void DoPassthrough(AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString)
        {
            delegateFunction(this, Tag, Format, Args, FormattedString);
        }
    private:
        Delegate delegateFunction;
    };

}

#endif // !ASHLOGGER_LOGGERPASSAGE_H
