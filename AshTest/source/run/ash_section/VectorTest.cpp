#include "VectorTest.h"

#include <Ash/AshCRC32.h>
#include <AshObjects/AshVector.h>
#include <AshObjects/AshPrimitive.h>

#include <iostream>

static std::vector<int> smTestVector = {
    1,
    2,
    4,
    8,
    16,
    32,
    64
};

ash::AshResult VectorTest::Do()
{
    ash::AshBuffer* buffer = new ash::AshBuffer();
    ash::AshCRC32 expectedCRC = 0;

    // Write
    {
        ash::AshStream* bufferStream = new ash::AshStream(buffer, ash::AshStreamMode::WRITE);
        ash::objects::AshVector<ash::objects::AshPrimitive<int>> testVector = ash::objects::AshVector<ash::objects::AshPrimitive<int>>();

        int i = 0;

        for(int c : smTestVector)
        {
            testVector.push_back(c);
            ++i;

            unsigned long long l = i | (c << 16);

            expectedCRC.Update(&l, sizeof(l));
        }

        buffer->AllocateSize(testVector.GetExportSize());
        
        if(testVector.Export(bufferStream) == false)
        {
            return ash::AshResult(false, "The vector testVector export failed.");
        }

        delete bufferStream;
    }

    // Read
    {
        ash::AshStream* bufferStream = new ash::AshStream(buffer, ash::AshStreamMode::READ);
        ash::objects::AshVector<ash::objects::AshPrimitive<int>> testVector = ash::objects::AshVector<ash::objects::AshPrimitive<int>>();
        ash::AshCRC32 calculatedCRC = ash::AshCRC32();

        if(testVector.Import(bufferStream) == false)
        {
            return ash::AshResult(false, "The vector testVector import failed.");
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
            return ash::AshResult(false, "The calculatedCRC and expectedCRC did not match.");
        }

        delete bufferStream;
    }

    return ash::AshResult(true);
}