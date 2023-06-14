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
        virtual void DoPassthrough(AshLoggerTag Tag, std::string Format, fmt::format_args Args) {}
        virtual void ApplyFilterForPassage(AshLoggerTagFilter LoggerTagFilter) {}
    };

    class AshLoggerDefaultPassage : public AshLoggerPassage
    {
    public:
        AshLoggerDefaultPassage():
            parent(nullptr),
            parentRegistrationName(),
            LoggerTagFilter()
        {}

        void SetParent(AshLogger* Parent, std::string ParentRegistrationName)
        {
            parent = Parent;
            parentRegistrationName = ParentRegistrationName;
        }

        void ApplyFilterForPassage(AshLoggerTagFilter LoggerTagFilter)
        {
            LoggerTagFilter = LoggerTagFilter;
        }

        virtual AshLogger* GetParent() { return parent; }
        virtual std::string GetParentRegistrationName() {return parentRegistrationName; }
        virtual AshLoggerTagFilter GetLoggerTagFilter() { return LoggerTagFilter; }
    private:
        AshLogger* parent; 
        std::string parentRegistrationName;
        AshLoggerTagFilter LoggerTagFilter;
    };

    class AshLoggerFunctionPassage : public AshLoggerDefaultPassage
    {
    public:
        using Delegate = std::function<void(AshLoggerDefaultPassage* This, AshLoggerTag Tag, std::string Format, fmt::format_args Args)>;

        AshLoggerFunctionPassage(Delegate DelegateFunction): delegateFunction(DelegateFunction) {}
        ~AshLoggerFunctionPassage() = default;

        void DoPassthrough(AshLoggerTag Tag, std::string Format, fmt::format_args Args) 
        {
            delegateFunction(this, Tag, Format, Args);
        }
    private:
        Delegate delegateFunction;
    };

}