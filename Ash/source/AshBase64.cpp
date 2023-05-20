#include "Ash/AshBase64.h"
#include <vector>

// AshBase64 implementation being used from: https://stackoverflow.com/a/13935718 (https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c)

namespace ash
{

    static constexpr std::string_view smBase64Chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
    
    static inline bool IsCharBase64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    std::string AshBase64::Encode(AshBuffer* Buffer)
    {
        std::string ret = "";
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        AshSize bufLen = Buffer->GetSize();
        unsigned char* buf = Buffer->GetBytes();

        while (bufLen--) 
        {
            char_array_3[i++] = *(buf++);
            if (i == 3) 
            {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; (i <4) ; i++)
                    ret += smBase64Chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i)
        {
            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                ret += smBase64Chars[char_array_4[j]];

            while((i++ < 3))
                ret += '=';
        }

        return ret;
    }

    AshBuffer* AshBase64::Decode(std::string Text)
    {
        size_t in_len = Text.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::vector<unsigned char> ret = {};

        while (in_len-- && ( Text[in_] != '=') && IsCharBase64(Text[in_])) 
        {
            char_array_4[i++] = Text[in_]; in_++;
            if (i ==4) 
            {
                for (i = 0; i <4; i++)
                char_array_4[i] = (unsigned char)smBase64Chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret.push_back(char_array_3[i]);
                i = 0;
            }
            }

        if (i) 
        {
            for (j = i; j <4; j++)
                char_array_4[j] = 0;

            for (j = 0; j <4; j++)
                char_array_4[j] = (unsigned char)smBase64Chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
        }

        AshBuffer* returnBuffer = new AshBuffer();
        returnBuffer->ImportBytesVector(ret);
        return returnBuffer;
    }

}