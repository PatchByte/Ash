#pragma once

#include <iostream>
#include <future>
#include <malloc.h>

namespace ash
{

    class AshResult
    {
    public:
        AshResult(bool Successful): m_Success(Successful), m_Message("") {}
        AshResult(bool Successful, std::string Message): m_Success(Successful), m_Message(Message) {}
        virtual  ~AshResult() = default;

        virtual bool WasSuccessful() { return m_Success; }
        virtual bool HasError() { return m_Success == false; }
        virtual std::string GetMessage() { return m_Message; }
    private:
        bool m_Success;
        std::string m_Message;
    };

    class AshPointerResult : public AshResult
    {
    public:
        AshPointerResult(bool Successful): AshResult::AshResult(Successful) {}
        AshPointerResult(bool Successful, std::string Message, void* Result): AshResult::AshResult(Successful, Message), m_Result(Result) {}
    
        void* GetRawResult() { return m_Result; }
        template<typename T>
        T* GetResult() { return static_cast<T*>(m_Result); }
    private:
        void* m_Result;
    };

    template<typename T>
    class AshCustomResult : public AshResult
    {
    public:
        AshCustomResult(bool Successful): AshResult::AshResult(Successful) {}
        AshCustomResult(bool Successful, std::string Message): AshResult::AshResult(Successful, Message) {}

        AshCustomResult AttachResult(T Result)
        {
            m_Result = Result;
            return *this;
        }

        T GetResult() { return m_Result; }
    private:
        T m_Result;
    };
    
}
