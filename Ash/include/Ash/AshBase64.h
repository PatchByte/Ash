#ifndef _ASHBASE64_H
#define _ASHBASE64_H

#include "AshBuffer.h"
#include <iostream>

// AshBase64 implementation being used from: https://stackoverflow.com/a/13935718 (https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c)

namespace ash
{

    class Base64
    {
    public:
        static std::string Encode(Buffer* Buffer);
        static Buffer* Decode(std::string Text);
        static inline std::string Encode(Buffer& Buffer) { return Base64::Encode(&Buffer); }
    };

}

#endif // !_ASHBASE64_H
