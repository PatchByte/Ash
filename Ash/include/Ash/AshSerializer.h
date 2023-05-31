#pragma once
#include "AshBuffer.h"

namespace ash
{

    class AshSerializer
    {
    public:
        static std::string Encode(AshBuffer* Buffer, char EncodeShuffle = 'A');
        static AshBuffer* Decode(std::string Text, char DecodeShuffle = 'A');
    };

}