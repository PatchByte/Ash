#pragma once
#include "AshBuffer.h"

#include <vector>

/*
 * Implementation stolen from https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635
 */

namespace ash
{

    using AshCRC32Value = unsigned int;
    using AshCRC32Table = std::vector<AshCRC32Value>;

    class AshCRC32Utils
    {
    public:
        static AshCRC32Table GenerateTable();
        static AshCRC32Value UpdateWithCustomTable(AshCRC32Table Table, AshCRC32Value InitialValue, AshBuffer* Buffer);
        static AshCRC32Value Calculate(AshCRC32Value InitialValue, AshBuffer* Buffer);
    };

    class AshCRC32
    {
    public:
        AshCRC32();
        AshCRC32(AshCRC32Value InitialValue);

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

        inline AshCRC32Value GetValue() { return crcValue; }
    private:
        AshCRC32Table crcTable;
        AshCRC32Value crcValue;
    };

}