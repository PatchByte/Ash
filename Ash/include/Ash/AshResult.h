#pragma once

#include <iostream>
#include <future>

namespace ash
{

    class AshResult
    {
    public:
        AshResult(bool Successful): success(Successful), message("") {}
        AshResult(bool Successful, std::string Message): success(Successful), message(Message) {}
        virtual  ~AshResult() = default;

        virtual bool WasSuccessful() { return success; }
        virtual bool HasError() { return success == false; }
        virtual std::string GetMessage() { return message; }
    private:
        bool success;
        std::string message;
    };

    class AshPointerResult : public AshResult
    {
    public:
        AshPointerResult(bool Successful): AshResult::AshResult(Successful) {}
        AshPointerResult(bool Successful, std::string Message, void* Result): AshResult::AshResult(Successful, Message), result(Result) {}
    
        void* GetRawResult() { return result; }
        template<typename T>
        T* GetResult() { return static_cast<T*>(result); }
    private:
        void* result;
    };

    template<typename T>
    class AshCustomResult : public AshResult
    {
    public:
        AshCustomResult(bool Successful): AshResult::AshResult(Successful) {}
        AshCustomResult(bool Successful, std::string Message): AshResult::AshResult(Successful, Message) {}

        AshCustomResult AttachResult(T Result)
        {
            result = Result;
            return *this;
        }

        T GetResult() { return result; }
    private:
        T result;
    };

}