#pragma once
#include "AshBuffer.h"

#include <vector>

/*
 * Implementation stolen from https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635
 */

namespace ash
{

    using CRC32 = unsigned int;
    using CRC32Table = std::vector<CRC32>;

    class AshCRC32Utils
    {
    public:
        static CRC32Table GenerateTable();
        static CRC32 UpdateWithCustomTable(CRC32Table Table, CRC32 InitialValue, AshBuffer* Buffer);
        static CRC32 Calculate(CRC32 InitialValue, AshBuffer* Buffer);
    };

    class AshCRC32
    {
    public:
        AshCRC32();
        AshCRC32(CRC32 InitialValue);

        bool Init();
        inline bool Clear() { crcValue = 0; return true; }
        inline bool InitAndClear() { bool r = Init(); r &= Clear(); return r; }
        bool Update(ash::AshBuffer* Buffer);
        bool Update(void* BufferPointer, AshSize BufferSize);
        
        template<typename T>
        bool UpdateWithTemplateValue(T Value)
        {
            return this->Update(&Value, sizeof(Value));
        }

        inline CRC32 GetValue() { return crcValue; }
    private:
        CRC32Table crcTable;
        CRC32 crcValue;
    };

}