#include "Ash/CRC32.h"

/*
 * Implementation stolen from https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635
 */

namespace ash
{
    static constexpr size_t smAshCRC32TableSize = 256;

    // AshCRC32Utils

    CRC32Table CRC32Utils::GenerateTable()
    {
        CRC32Table table = {};

        uint32_t polynomial = 0xEDB88320;
        for (uint32_t i = 0; i < smAshCRC32TableSize; i++)
        {
            uint32_t c = i;

            for (size_t j = 0; j < 8; j++)
            {
                if (c & 1) {
                    c = polynomial ^ (c >> 1);
                }
                else
                {
                    c >>= 1;
                }
            }

            table.push_back(c);
        }

        return table;
    }

    CRC32 CRC32Utils::UpdateWithCustomTable(CRC32Table Table, CRC32 InitialValue, Buffer* Buffer)
    {
        uint32_t c = InitialValue ^ 0xFFFFFFFF;
		uint8_t* u = Buffer->GetBytes();
		for (size_t i = 0; i < Buffer->GetSize(); ++i)
		{
			c = Table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
		}
		return c ^ 0xFFFFFFFF;
    }

    CRC32 CRC32Utils::Calculate(CRC32 InitialValue, Buffer* Buffer)
    {
        CRC32Table table = CRC32Utils::GenerateTable();
        return CRC32Utils::UpdateWithCustomTable(table, InitialValue, Buffer);
    }

    // AshCRC32

    CRC32State::CRC32State():
        m_CrcValue(0)
    {
        this->Init();
    }

    CRC32State::CRC32State(CRC32 Value):
        m_CrcValue(Value)
    {
        this->Init();
    }

    bool CRC32State::Init()
    {
        this->m_CrcTable = CRC32Utils::GenerateTable();
        return true;
    }

    bool CRC32State::Update(ash::Buffer* Buffer)
    {
        this->m_CrcValue = CRC32Utils::UpdateWithCustomTable(this->m_CrcTable, this->m_CrcValue, Buffer);
        return true;
    }

    bool CRC32State::Update(void* BufferPointer, Size BufferSize)
    {
        Buffer* buffer = new Buffer(BufferPointer, BufferSize);

        bool result = this->Update(buffer);

        delete buffer;
        return result;
    }
}
