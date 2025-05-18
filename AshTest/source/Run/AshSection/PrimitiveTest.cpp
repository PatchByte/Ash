#include "PrimitiveTest.h"
#include "Ash/AshStream.h"

#include <AshObjects/AshPrimitive.h>

static constexpr double smDoubleValueExpected = 9.F;

ash::Result PrimitiveTest::Do()
{
    ash::Buffer* buffer = nullptr;

    // Write
    {
        ash::StreamExpandableExportBuffer* bufferStream = new ash::StreamExpandableExportBuffer();
        ash::objects::AshPrimitive<double> doublePrimitive = ash::objects::AshPrimitive<double>(smDoubleValueExpected);

        if(doublePrimitive.Export(bufferStream) == false)
        {
            return ash::Result(false, "AshPrimitive (double) failed to export.");
        }

        buffer = bufferStream->MakeCopyOfBuffer();

        delete bufferStream;
    }

    // Read
    {
        ash::Stream* bufferStream = new ash::StreamStaticBuffer(buffer, ash::StreamMode::READ);
        ash::objects::AshPrimitive<double> doublePrimitive = ash::objects::AshPrimitive<double>();

        if(doublePrimitive.Import(bufferStream) == false)
        {
            return ash::Result(false, "AshPrimitive (double) failed to import.");
        }

        if(doublePrimitive.GetDataValue() != smDoubleValueExpected)
        {
            return ash::Result(false, "AshPrimitive (double) failed to match expected value.");
        }

        delete bufferStream;
    }

    return ash::Result(true);
}
