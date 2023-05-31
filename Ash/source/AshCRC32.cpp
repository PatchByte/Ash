#include "Ash/AshCRC32.h"
#include <iostream>

/*
 * Implementation stolen from https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635
 */

namespace ash
{
    static constexpr size_t smCrc32TableSize = 256;

    // AshCRC32Utils

    CRC32Table AshCRC32Utils::GenerateTable()
    {
        CRC32Table table = {};

        uint32_t polynomial = 0xEDB88320;
		for (uint32_t i = 0; i < smCrc32TableSize; i++) 
		{
			uint32_t c = i;
			for (size_t j = 0; j < 8; j++) 
			{
				if (c & 1) {
					c = polynomial ^ (c >> 1);
				}
				else {
					c >>= 1;
				}
			}

            table.push_back(c);
		}

        return table;
    }

    CRC32 AshCRC32Utils::UpdateWithCustomTable(CRC32Table Table, CRC32 InitialValue, AshBuffer* Buffer)
    {
        uint32_t c = InitialValue ^ 0xFFFFFFFF;
		uint8_t* u = Buffer->GetBytes();
		for (size_t i = 0; i < Buffer->GetSize(); ++i) 
		{
			c = Table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
		}
		return c ^ 0xFFFFFFFF;
    }

    CRC32 AshCRC32Utils::Calculate(CRC32 InitialValue, AshBuffer* Buffer)
    {
        CRC32Table table = AshCRC32Utils::GenerateTable();
        return AshCRC32Utils::UpdateWithCustomTable(table, InitialValue, Buffer);
    }

    // AshCRC32

    AshCRC32::AshCRC32():
        crcValue(0)
    {
        this->Init();
    }

    AshCRC32::AshCRC32(CRC32 Value):
        crcValue(Value)
    {
        this->Init();
    }

    bool AshCRC32::Init()
    {
        this->crcTable = AshCRC32Utils::GenerateTable();
        return true;
    }
    
    bool AshCRC32::Update(ash::AshBuffer* Buffer)
    {
        this->crcValue = AshCRC32Utils::UpdateWithCustomTable(this->crcTable, this->crcValue, Buffer);
        return true;
    }

    bool AshCRC32::Update(void* BufferPointer, AshSize BufferSize)
    {
        AshBuffer* buffer = new AshBuffer(BufferPointer, BufferSize);

        bool result = this->Update(buffer);

        delete buffer;
        return result;
    }
}