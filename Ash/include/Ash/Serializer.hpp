#ifndef ASH_SERIALIZER_H
#define ASH_SERIALIZER_H

#include "Buffer.hpp"

namespace ash
{

    class TextSerializer
    {
    public:
        static std::string Encode(void* Data, Size Length, char Shuffle = 'A');

        static inline std::string Encode(Buffer* Buffer, char Shuffle = 'A')
        {
            return Encode(Buffer->GetPointer(), Buffer->GetSize(), Shuffle);
        }

        static inline std::string Encode(Buffer& Buffer, char Shuffle = 'A')
        {
            return Encode(Buffer.GetPointer(), Buffer.GetSize(), Shuffle);
        }

        static bool Decode(std::string Text, void* Data, Size Length, char Shuffle = 'A');

        static Buffer* Decode(std::string Text, char Shuffle = 'A');
    };

} // namespace ash

#endif // !ASH_SERIALIZER_H
