#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>

namespace ash::objects
{
    using AshStringLength = unsigned short;

    // Secure String
    class AshAsciiString : public AshStreamableObject
    {
    public:
        AshAsciiString();
        AshAsciiString(std::string Text);
        AshAsciiString(std::string_view Text);
        AshAsciiString(const char* Text);
        
        inline void operator=(std::string Text)
        {
            text = Text;
        }

        inline void operator=(std::string_view Text)
        {
            text = Text;
        }

        inline void operator=(const char* Text)
        {
            text = Text;
        }

        inline AshStringLength GetTextLimitLength()
        {
            return textLimitLength;
        }

        inline std::string GetText()
        {
            return text;
        }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshAsciiString");
    private:
        std::string text;
        AshStringLength textLimitLength;
    };

    class AshWideString : public AshStreamableObject 
    {
    public:
        AshWideString();
        AshWideString(std::wstring Text);
        AshWideString(std::wstring_view Text);
        AshWideString(const wchar_t* Text);

        inline void operator=(std::wstring Text)
        {
            text = Text;
        }

        inline void operator=(std::wstring_view Text)
        {
            text = Text;
        }

        inline void operator=(const wchar_t* Text)
        {
            text = Text;
        }

        inline AshStringLength GetTextLimitLength()
        {
            return textLimitLength;
        }

        inline std::wstring GetText()
        {
            return text;
        }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshWideString");
    private:
        std::wstring text;
        AshStringLength textLimitLength;
    };

}