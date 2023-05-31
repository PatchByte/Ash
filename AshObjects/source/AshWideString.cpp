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
        textLimitLength(smAshStringLimitLength)
    {}

    AshWideString::AshWideString(std::wstring Text):
        textLimitLength(smAshStringLimitLength),
        text(Text)
    {}

    AshWideString::AshWideString(std::wstring_view Text):
        textLimitLength(smAshStringLimitLength),
        text(Text)
    {}

    AshWideString::AshWideString(const wchar_t* Text):
        textLimitLength(smAshStringLimitLength),
        text(Text)
    {}

    bool AshWideString::Import(ash::AshStream* Stream)
    {
        unsigned short textSeed = Stream->Read<unsigned short>();
        AshStringLength textLength = Stream->Read<AshStringLength>();

        this->text.clear();

        if(textLength > textLimitLength)
        {
            text = L"Text overflow.";
            return false;
        }

        for(AshStringLength i = 0; i < textLength; i++)
        {
            wchar_t c = Stream->Read<wchar_t>();
            c ^= textSeed;
            this->text += (c);
        }

        return Stream->HasErrorOccurred() == false;
    }

    bool AshWideString::Export(ash::AshStream* Stream)
    {
        std::random_device rd;
        std::mt19937 engine(rd());
        std::uniform_int_distribution<int> distribution(1, 256 * 256);
        unsigned short textSeed = (unsigned short)distribution(engine);

        Stream->Write<unsigned short>(textSeed);
        Stream->Write<AshStringLength>((AshStringLength)(text.length()));

        for(auto c : text)
        {
            Stream->Write<wchar_t>(c ^ textSeed);
        }

        return Stream->HasErrorOccurred() == false;
    }

    AshSize AshWideString::GetExportSize()
    {
        return (
            sizeof(unsigned short) +
            sizeof(AshStringLength) +
            sizeof(wchar_t) * text.length()
        );
    }

}