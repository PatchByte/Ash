#ifndef ASHLOGGER_LOGGERPASSAGE_H
#define ASHLOGGER_LOGGERPASSAGE_H

#include <fmt/core.h>
#include <functional>
#include <iostream>

#include "LoggerTag.h"

namespace ash::logger
{

    class Logger;

    class Passage
    {
    public:
        virtual ~Passage() = default;

        virtual void SetParent(Logger* Parent, std::string ParentRegistrationName)
        {
        }

        virtual void DoPassthrough(Tag Tag, std::string Format, fmt::format_args Args, std::string FormattedString)
        {
        }

        virtual void ApplyFilterForPassage(TagFilter LoggerTagFilter)
        {
        }
    };

    class DefaultPassage : public Passage
    {
    public:
        DefaultPassage() : m_Parent(nullptr), m_ParentRegistrationName(), m_LoggerTagFilter()
        {
        }

        void SetParent(Logger* Parent, std::string ParentRegistrationName)
        {
            m_Parent = Parent;
            m_ParentRegistrationName = ParentRegistrationName;
        }

        void ApplyFilterForPassage(TagFilter LoggerTagFilter)
        {
            m_LoggerTagFilter = LoggerTagFilter;
        }

        virtual Logger* GetParent()
        {
            return m_Parent;
        }

        virtual std::string GetParentRegistrationName()
        {
            return m_ParentRegistrationName;
        }

        virtual TagFilter GetLoggerTagFilter()
        {
            return m_LoggerTagFilter;
        }

    private:
        Logger* m_Parent;
        std::string m_ParentRegistrationName;
        TagFilter m_LoggerTagFilter;
    };

    class FunctionPassage : public DefaultPassage
    {
    public:
        using Delegate = std::function<void(DefaultPassage* This, Tag Tag, std::string Format, fmt::format_args Args, std::string FormattedString)>;

        FunctionPassage(Delegate DelegateFunction) : m_DelegateFunction(DelegateFunction)
        {
        }

        ~FunctionPassage() = default;

        void DoPassthrough(Tag Tag, std::string Format, fmt::format_args Args, std::string FormattedString)
        {
            m_DelegateFunction(this, Tag, Format, Args, FormattedString);
        }

    private:
        Delegate m_DelegateFunction;
    };

} // namespace ash

#endif // !ASHLOGGER_LOGGERPASSAGE_H
