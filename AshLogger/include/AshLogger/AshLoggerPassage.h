#pragma once
#include <iostream>
#include <functional>
#include <fmt/core.h>

namespace ash
{

    class AshLogger;

    class AshLoggerPassage
    {
    public:
        virtual ~AshLoggerPassage() = default;
        virtual void SetParent(AshLogger* Parent, std::string ParentRegistrationName) {}
        virtual void DoPassthrough(std::string Format, fmt::format_args Args) {}
    };

    class AshLoggerDefaultPassage : public AshLoggerPassage
    {
    public:
        void SetParent(AshLogger* Parent, std::string ParentRegistrationName)
        {
            parent = Parent;
            parentRegistrationName = ParentRegistrationName;
        }

        virtual AshLogger* GetParent() { return parent; }
        virtual std::string GetParentRegistrationName() {return parentRegistrationName; }
    private:
        AshLogger* parent; 
        std::string parentRegistrationName;
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