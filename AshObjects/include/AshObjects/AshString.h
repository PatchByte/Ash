#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>
#include <utility>

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
        
        inline AshAsciiString& operator=(std::string Text)
        {
            m_Text = std::move(Text);
            return *this;
        }

        inline AshAsciiString& operator=(std::string_view Text)
        {
            m_Text = Text;
            return *this;
        }

        inline AshAsciiString& operator=(const char* Text)
        {
            m_Text = Text;
            return *this;
        }

        inline AshStringLength GetTextLimitLength()
        {
            return m_TextLimitLength;
        }

        inline void SetTextLimitLength(AshStringLength TextLimitLength)
        {
            m_TextLimitLength = TextLimitLength;
        }

        inline std::string GetText()
        {
            return m_Text;
        }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshAsciiString");
    private:
        std::string m_Text;
        AshStringLength m_TextLimitLength;
    };

    class AshWideString : public AshStreamableObject 
    {
    public:
        AshWideString();
        AshWideString(std::wstring Text);
        AshWideString(std::wstring_view Text);
        AshWideString(const wchar_t* Text);

        inline AshWideString& operator=(std::wstring Text)
        {
            m_Text = std::move(Text);
            return *this;
        }

        inline AshWideString& operator=(std::wstring_view Text)
        {
            m_Text = Text;
            return *this;
        }

        inline AshWideString& operator=(const wchar_t* Text)
        {
            m_Text = Text;
            return *this;
        }

        inline AshStringLength GetTextLimitLength()
        {
            return m_TextLimitLength;
        }

        inline void SetTextLimitLength(AshStringLength TextLimitLength)
        {
            m_TextLimitLength = TextLimitLength;
        }

        inline std::wstring GetText()
        {
            return m_Text;
        }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshWideString");
    private:
        std::wstring m_Text;
        AshStringLength m_TextLimitLength;
    };

}