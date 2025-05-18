#pragma once
#include "Buffer.h"

namespace ash
{

    class TextSerializer
    {
    public:
        static std::string Encode(Buffer* Buffer, char EncodeShuffle = 'A');
        static std::string Encode(Buffer& Buffer, char EncodeShuffle = 'A') { return Encode(&Buffer, EncodeShuffle); }
        static Buffer* Decode(std::string Text, char DecodeShuffle = 'A');
    };

}
