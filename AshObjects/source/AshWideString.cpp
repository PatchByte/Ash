#include "AshObjects/AshString.h"
#include <random>

/*
 * AshString Recipe
 *
 * <unsigned char : 1 : text seed for xor>
 * <unsigned short : 2 : text length>
 * <char[] : 1 * text length : text content>
 *
 */

namespace ash::objects
{

    static constexpr AshStringLength smAshStringLimitLength = 512;

    // AshWideString

    AshWideString::AshWideString():
        m_TextLimitLength(smAshStringLimitLength)
    {}

    AshWideString::AshWideString(std::wstring Text):
        m_TextLimitLength(smAshStringLimitLength),
        m_Text(Text)
    {}

    AshWideString::AshWideString(std::wstring_view Text):
        m_TextLimitLength(smAshStringLimitLength),
        m_Text(Text)
    {}

    AshWideString::AshWideString(const wchar_t* Text):
        m_TextLimitLength(smAshStringLimitLength),
        m_Text(Text)
    {}

    bool AshWideString::Import(ash::Stream* Stream)
    {
        unsigned short textSeed = Stream->Read<unsigned short>();
        AshStringLength textLength = Stream->Read<AshStringLength>();

        this->m_Text.clear();

        if(textLength > m_TextLimitLength)
        {
            m_Text = L"Text overflow.";
            return false;
        }

        for(AshStringLength i = 0; i < textLength; i++)
        {
            wchar_t c = Stream->Read<wchar_t>();
            c ^= textSeed;
            this->m_Text += (c);
        }

        return Stream->HasErrorOccurred() == false;
    }

    bool AshWideString::Export(ash::Stream* Stream)
    {
        unsigned short textSeed = 0;

        Stream->Write<unsigned short>(textSeed);
        Stream->Write<AshStringLength>((AshStringLength)(m_Text.length()));

        for(auto c : m_Text)
        {
            Stream->Write<wchar_t>(c ^ textSeed);
        }

        return Stream->HasErrorOccurred() == false;
    }
}
