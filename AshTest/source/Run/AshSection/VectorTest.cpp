#include "VectorTest.h"

#include <Ash/AshCRC32.h>
#include <AshObjects/AshVector.h>
#include <AshObjects/AshPrimitive.h>
#include <Ash/AshStream.h>

static std::vector<int> smTestVector = {
    1,
    2,
    4,
    8,
    16,
    32,
    64
};

ash::Result VectorTest::Do()
{
    ash::Buffer* buffer = nullptr;
    ash::CRC32State expectedCRC = 0;

    // Write
    {
        ash::StreamExpandableExportBuffer* bufferStream = new ash::StreamExpandableExportBuffer();
        ash::objects::AshVector<ash::objects::AshPrimitive<int>> testVector = ash::objects::AshVector<ash::objects::AshPrimitive<int>>();

        int i = 0;

        for(int c : smTestVector)
        {
            testVector.push_back(c);
            ++i;

            unsigned long long l = i | (c << 16);

            expectedCRC.Update(&l, sizeof(l));
        }

        if(testVector.Export(bufferStream) == false)
        {
            return ash::Result(false, "The vector testVector export failed.");
        }

        buffer = bufferStream->MakeCopyOfBuffer();

        delete bufferStream;
    }

    // Read
    {
        ash::Stream* bufferStream = new ash::StreamStaticBuffer(buffer, ash::StreamMode::READ);
        ash::objects::AshVector<ash::objects::AshPrimitive<int>> testVector = ash::objects::AshVector<ash::objects::AshPrimitive<int>>();
        ash::CRC32State calculatedCRC = ash::CRC32State();

        if(testVector.Import(bufferStream) == false)
        {
            return ash::Result(false, "The vector testVector import failed.");
        }

        int i = 0;

        for(auto c : testVector)
        {
            ++i;
            unsigned long long l = i | (c.GetDataValue() << 16);
            calculatedCRC.Update(&l, sizeof(l));
        }

        if(calculatedCRC.GetValue() != expectedCRC.GetValue())
        {
            return ash::Result(false, "The calculatedCRC and expectedCRC did not match.");
        }

        delete bufferStream;
    }

    return ash::Result(true);
}
