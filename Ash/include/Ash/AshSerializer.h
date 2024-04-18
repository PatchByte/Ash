#pragma once
#include "AshBuffer.h"

namespace ash
{

    class AshSerializer
    {
    public:
        static std::string Encode(AshBuffer* Buffer, char EncodeShuffle = 'A');
        static std::string Encode(AshBuffer& Buffer, char EncodeShuffle = 'A') { return Encode(&Buffer, EncodeShuffle); }
        static AshBuffer* Decode(std::string Text, char DecodeShuffle = 'A');
    };

}
