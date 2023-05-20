#pragma once
#include "AshBuffer.h"
#include <iostream>

// AshBase64 implementation being used from: https://stackoverflow.com/a/13935718 (https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c)

namespace ash
{

    class AshBase64
    {
    public:
        static std::string Encode(AshBuffer* Buffer);
        static AshBuffer* Decode(std::string Text);
    };

}