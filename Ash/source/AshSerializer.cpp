#include "Ash/AshSerializer.h"

namespace ash
{

    std::string AshSerializer::Encode(AshBuffer* Buffer, char EncodeShuffle)
    {
        std::string Result = "";

        for(size_t i = 0; i < Buffer->GetSize(); i++)
        {
            unsigned char currentByte = Buffer->GetBytes()[i];

            char firstHalfPartOfByte =  EncodeShuffle + ((currentByte >> 0) & 0xF);
            char secondHalfPartOfByte = EncodeShuffle + ((currentByte >> 4) & 0xF);

            Result += firstHalfPartOfByte;
            Result += secondHalfPartOfByte;
        }

        return Result;
    }
    
    AshBuffer* AshSerializer::Decode(std::string Text, char DecodeShuffle)
    {
        if(Text.length() <= 0)
        {
            return nullptr;
        }

        AshBuffer* buffer = new AshBuffer();
        size_t bufferSize = Text.length() / 2;

        if((bufferSize * 2) != Text.length())
        {
            return nullptr;
        }

        if(buffer->AllocateSize(bufferSize) == false)
        {
            return nullptr;
        }

        for(size_t i = 0; i < bufferSize; i++)
        {
            unsigned char currentByte = 0;
            
            currentByte |= (Text[(i * 2) + 0] - DecodeShuffle) << 0;
            currentByte |= (Text[(i * 2) + 1] - DecodeShuffle) << 4;

            buffer->GetBytes()[i] = currentByte;
        }

        return buffer;
    }

}