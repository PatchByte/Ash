#ifndef ASH_RESULT_H
#define ASH_RESULT_H

#include <future>
#include <iostream>
#include <malloc.h>

namespace ash
{

    class Result
    {
    public:
        inline Result(bool Successful) : m_Success(Successful), m_Message("")
        {
        }

        inline Result(bool Successful, std::string Message) : m_Success(Successful), m_Message(Message)
        {
        }

        virtual ~Result() = default;

        virtual bool WasSuccessful()
        {
            return m_Success;
        }

        virtual bool HasError()
        {
            return m_Success == false;
        }

        virtual std::string GetMessage()
        {
            return m_Message;
        }

    private:
        bool m_Success;
        std::string m_Message;
    };

    class PointerResult : public Result
    {
    public:
        PointerResult(bool Successful) : Result::Result(Successful)
        {
        }

        PointerResult(bool Successful, std::string Message, void* Result) : Result::Result(Successful, Message), m_Result(Result)
        {
        }

        void* AsRaw()
        {
            return m_Result;
        }

        template <typename T>
        T* As()
        {
            return static_cast<T*>(m_Result);
        }

    private:
        void* m_Result;
    };

    template <typename T>
    class CustomResult : public Result
    {
    public:
        CustomResult(bool Successful) : Result::Result(Successful)
        {
        }

        CustomResult(bool Successful, std::string Message) : Result::Result(Successful, Message)
        {
        }

        CustomResult Attach(T Result)
        {
            m_Result = Result;
            return *this;
        }

        T Get()
        {
            return m_Result;
        }

    private:
        T m_Result;
    };

} // namespace ash

#endif // !ASH_RESULT_H
