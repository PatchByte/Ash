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
        virtual void DoPassthrough(std::string Format, fmt::format_args Args) {}
        virtual void ApplyFilterForPassage(AshLoggingTagFilter LoggingTagFilter) {}
    };

    class AshLoggerDefaultPassage : public AshLoggerPassage
    {
    public:
        AshLoggerDefaultPassage():
            parent(nullptr),
            parentRegistrationName(),
            loggingTagFilter()
        {}

        void SetParent(AshLogger* Parent, std::string ParentRegistrationName)
        {
            parent = Parent;
            parentRegistrationName = ParentRegistrationName;
        }

        void ApplyFilterForPassage(AshLoggingTagFilter LoggingTagFilter)
        {
            loggingTagFilter = LoggingTagFilter;
        }

        virtual AshLogger* GetParent() { return parent; }
        virtual std::string GetParentRegistrationName() {return parentRegistrationName; }
        virtual AshLoggingTagFilter GetLoggingTagFilter() { return loggingTagFilter; }
    private:
        AshLogger* parent; 
        std::string parentRegistrationName;
        AshLoggingTagFilter loggingTagFilter;
    };

    class AshLoggerFunctionPassage : public AshLoggerDefaultPassage
    {
    public:
        using Delegate = std::function<void(AshLoggerDefaultPassage* This, std::string Format, fmt::format_args Args)>;

        AshLoggerFunctionPassage(Delegate DelegateFunction): delegateFunction(DelegateFunction) {}
        ~AshLoggerFunctionPassage() = default;

        void DoPassthrough(std::string Format, fmt::format_args Args) 
        {
            delegateFunction(this, Format, Args);
        }
    private:
        Delegate delegateFunction;
    };

}