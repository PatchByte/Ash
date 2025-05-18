#ifndef ASH_CRC32_H
#define ASH_CRC32_H

#include <vector>

#include "Buffer.h"
/*
 * Implementation stolen from https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635
 */

namespace ash
{

    using CRC32 = unsigned int;
    using CRC32Table = std::vector<CRC32>;

    class CRC32Utils
    {
    public:
        static CRC32Table GenerateTable();
        static CRC32 UpdateWithCustomTable(CRC32Table Table, CRC32 InitialValue, Buffer* Buffer);
        static CRC32 Calculate(CRC32 InitialValue, Buffer* Buffer);
    };

    class CRC32State
    {
    public:
        CRC32State();
        CRC32State(CRC32 InitialValue);

        bool Init();
        inline bool Clear() { m_CrcValue = 0; return true; }
        inline bool InitAndClear() { bool r = Init(); r &= Clear(); return r; }
        bool Update(ash::Buffer* Buffer);
        bool Update(ash::Buffer& Buffer) { return this->Update(static_cast<ash::Buffer*>(&Buffer)); }
        bool Update(void* BufferPointer, Size BufferSize);
        bool Update(const void* BufferPointer, Size BufferSize) { return this->Update(const_cast<void*>(BufferPointer), BufferSize); }

        template<typename T>
        bool UpdateWithPrimitive(T Value)
        {
            return this->Update(&Value, sizeof(Value));
        }

        inline CRC32 GetValue() { return m_CrcValue; }
    private:
        CRC32Table m_CrcTable;
        CRC32 m_CrcValue;
    };

}

#endif // !ASH_CRC32_H
