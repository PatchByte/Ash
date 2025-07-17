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

    bool TextSerializer::Decode(std::string Text, void* Data, Size Length, char Shuffle)
    {
        if (Text.length() <= 0 || (Text.length() % 2) != 0)
        {
            return false;
        }

        size_t bufferSize = Text.length() / 2;

        if ((bufferSize * 2) != Text.length())
        {
            return false;
        }

        for (size_t i = 0; i < bufferSize; i++)
        {
            unsigned char currentByte = 0;

            currentByte |= (Text[(i * 2) + 0] - Shuffle) << 0;
            currentByte |= (Text[(i * 2) + 1] - Shuffle) << 4;

            static_cast<unsigned char*>(Data)[i] = currentByte;
        }

        return true;
    }

    Buffer* TextSerializer::Decode(std::string Text, char DecodeShuffle)
    {
        if (Text.length() <= 0 || (Text.length() % 2) != 0)
        {
            return nullptr;
        }

        ash::Buffer* buffer = new ash::Buffer();

        if (buffer->AllocateBySize(Text.length() / 2) == false)
        {
            delete buffer;
            buffer = nullptr;

            return buffer;
        }

        if (Decode(Text, buffer->GetPointer(), buffer->GetSize()) == false)
        {
            delete buffer;
            buffer = nullptr;

            return buffer;
        }

        return buffer;
    }

} // namespace ash
