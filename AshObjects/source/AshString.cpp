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
        textLimitLength(smAshStringLimitLength)
    {}

    AshAsciiString::AshAsciiString(std::string Text):
        textLimitLength(smAshStringLimitLength),
        text(Text)
    {}

    AshAsciiString::AshAsciiString(std::string_view Text):
        textLimitLength(smAshStringLimitLength),
        text(Text)
    {}

    AshAsciiString::AshAsciiString(const char* Text):
        textLimitLength(smAshStringLimitLength),
        text(Text)
    {}

    bool AshAsciiString::Import(ash::AshStream* Stream)
    {
        unsigned char textSeed = Stream->Read<unsigned char>();
        AshStringLength textLength = Stream->Read<AshStringLength>();

        this->text.clear();

        if(textLength > textLimitLength)
        {
            text = "Text overflow.";
            return false;
        }

        for(AshStringLength i = 0; i < textLength; i++)
        {
            char c = Stream->Read<char>();
            c ^= textSeed;
            this->text += (c);
        }

        return Stream->HasErrorOccurred() == false;
    }

    bool AshAsciiString::Export(ash::AshStream* Stream)
    {
        std::random_device rd;
        std::mt19937 engine(rd());
        std::uniform_int_distribution<int> distribution(1, 255);
        unsigned char textSeed = (unsigned char)distribution(engine);

        Stream->Write<unsigned char>(textSeed);
        Stream->Write<AshStringLength>((AshStringLength)(text.length()));

        for(auto c : text)
        {
            Stream->Write<char>(c ^ textSeed);
        }

        return Stream->HasErrorOccurred() == false;
    }

    AshSize AshAsciiString::GetExportSize()
    {
        return (
            sizeof(unsigned char) +
            sizeof(AshStringLength) +
            text.length()
        );
    }

}