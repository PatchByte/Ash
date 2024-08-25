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

    // AshAsciiString

    AshAsciiString::AshAsciiString():
        m_TextLimitLength(smAshStringLimitLength)
    {}

    AshAsciiString::AshAsciiString(std::string Text):
        m_TextLimitLength(smAshStringLimitLength),
        m_Text(Text)
    {}

    AshAsciiString::AshAsciiString(std::string_view Text):
        m_TextLimitLength(smAshStringLimitLength),
        m_Text(Text)
    {}

    AshAsciiString::AshAsciiString(const char* Text):
        m_TextLimitLength(smAshStringLimitLength),
        m_Text(Text)
    {}

    bool AshAsciiString::Import(ash::AshStream* Stream)
    {
        unsigned char textSeed = Stream->Read<unsigned char>();
        AshStringLength textLength = Stream->Read<AshStringLength>();

        this->m_Text.clear();

        if(textLength > m_TextLimitLength)
        {
            m_Text = "Text overflow.";
            return false;
        }

        for(AshStringLength i = 0; i < textLength; i++)
        {
            char c = Stream->Read<char>();
            c ^= textSeed;
            this->m_Text += (c);
        }

        return Stream->HasErrorOccurred() == false;
    }

    bool AshAsciiString::Export(ash::AshStream* Stream)
    {
        unsigned char textSeed = 0;

        Stream->Write<unsigned char>(textSeed);
        Stream->Write<AshStringLength>((AshStringLength)(m_Text.length()));

        for(auto c : m_Text)
        {
            Stream->Write<char>(c ^ textSeed);
        }

        return Stream->HasErrorOccurred() == false;
    }
}
