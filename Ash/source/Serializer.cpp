#include "Ash/Serializer.hpp"
#include "Ash/Buffer.hpp"

namespace ash
{

    std::string TextSerializer::Encode(void* Data, Size Length, char EncodeShuffle)
    {
        std::string Result = "";

        for (Size i = 0; i < Length; i++)
        {
            unsigned char currentByte = static_cast<unsigned char*>(Data)[i];

            char firstHalfPartOfByte = EncodeShuffle + ((currentByte >> 0) & 0xF);
            char secondHalfPartOfByte = EncodeShuffle + ((currentByte >> 4) & 0xF);

            Result += firstHalfPartOfByte;
            Result += secondHalfPartOfByte;
        }

        return Result;
    }

    bool TextSerializer::Decode(std::string Text, void* OutputData, Size* OutputLength, char Shuffle)
    {
        if (Text.length() <= 0 || (Text.length() % 2) != 0)
        {
            return false;
        }

        size_t actualOutputSize = Text.length() >> 1;

        if (actualOutputSize > *OutputLength)
        {
            return false;
        }

        for (size_t i = 0; i < actualOutputSize; i++)
        {
            unsigned char currentByte = 0;

            currentByte |= (Text[(i * 2) + 0] - Shuffle) << 0;
            currentByte |= (Text[(i * 2) + 1] - Shuffle) << 4;

            static_cast<unsigned char*>(OutputData)[i] = currentByte;
        }

        *OutputLength = actualOutputSize;

        return true;
    }

    Buffer* TextSerializer::Decode(std::string Text, char DecodeShuffle)
    {
        if (Text.length() <= 0 || (Text.length() % 2) != 0)
        {
            return nullptr;
        }

        ash::Buffer* buffer = new ash::Buffer();

        if (buffer->AllocateBySize(Text.length() >> 1) == false)
        {
            delete buffer;
            buffer = nullptr;

            return buffer;
        }

        ash::Size bufferSize = buffer->GetSize();

        if (Decode(Text, buffer->GetPointer(), &bufferSize) == false)
        {
            delete buffer;
            buffer = nullptr;

            return buffer;
        }

        return buffer;
    }

} // namespace ash
