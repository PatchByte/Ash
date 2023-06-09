#pragma once
#include <iostream>
#include <functional>
#include <fmt/core.h>

#include "AshLoggerTag.h"

namespace ash
{

    class AshLogger;

    class AshLoggerPassage
    {
    public:
        virtual ~AshLoggerPassage() = default;
        virtual void SetParent(AshLogger* Parent, std::string ParentRegistrationName) {}
        virtual void DoPassthrough(AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString) {}
        virtual void ApplyFilterForPassage(AshLoggerTagFilter LoggerTagFilter) {}
    };

    class AshLoggerDefaultPassage : public AshLoggerPassage
    {
    public:
        AshLoggerDefaultPassage():
            parent(nullptr),
            parentRegistrationName(),
            loggerTagFilter()
        {}

        void SetParent(AshLogger* Parent, std::string ParentRegistrationName)
        {
            parent = Parent;
            parentRegistrationName = ParentRegistrationName;
        }

        void ApplyFilterForPassage(AshLoggerTagFilter LoggerTagFilter)
        {
            loggerTagFilter = LoggerTagFilter;
        }

        virtual AshLogger* GetParent() { return parent; }
        virtual std::string GetParentRegistrationName() {return parentRegistrationName; }
        virtual AshLoggerTagFilter GetLoggerTagFilter() { return loggerTagFilter; }
    private:
        AshLogger* parent; 
        std::string parentRegistrationName;
        AshLoggerTagFilter loggerTagFilter;
    };

    class AshLoggerFunctionPassage : public AshLoggerDefaultPassage
    {
    public:
        using Delegate = std::function<void(AshLoggerDefaultPassage* This, AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString)>;

        AshLoggerFunctionPassage(Delegate DelegateFunction): delegateFunction(DelegateFunction) {}
        ~AshLoggerFunctionPassage() = default;

        void DoPassthrough(AshLoggerTag Tag, std::string Format, fmt::format_args Args, std::string FormattedString) 
        {
            delegateFunction(this, Tag, Format, Args, FormattedString);
        }
    private:
        Delegate delegateFunction;
    };

}