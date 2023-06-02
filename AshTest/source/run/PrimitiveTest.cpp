#include "PrimitiveTest.h"

#include <AshObjects/AshPrimitive.h>

static constexpr double smDoubleValueExpected = 9.F;

ash::AshResult PrimitiveTest::Do()
{
    ash::AshBuffer* buffer = new ash::AshBuffer();

    // Write
    {
        ash::AshStream* bufferStream = new ash::AshStream(buffer, ash::AshStreamMode::WRITE);
        ash::objects::AshPrimitive<double> doublePrimitive = ash::objects::AshPrimitive<double>(smDoubleValueExpected);

        buffer->AllocateSize(doublePrimitive.GetExportSize());

        if(doublePrimitive.Export(bufferStream) == false)
        {
            return ash::AshResult(false, "AshPrimitive (double) failed to export.");
        }

        delete bufferStream;
    }

    // Read
    {
        ash::AshStream* bufferStream = new ash::AshStream(buffer, ash::AshStreamMode::READ);
        ash::objects::AshPrimitive<double> doublePrimitive = ash::objects::AshPrimitive<double>();

        if(doublePrimitive.Import(bufferStream) == false)
        {
            return ash::AshResult(false, "AshPrimitive (double) failed to import.");
        }

        if(doublePrimitive.GetDataValue() != smDoubleValueExpected)
        {
            return ash::AshResult(false, "AshPrimitive (double) failed to match expected value.");
        }

        delete bufferStream;
    }

    return ash::AshResult(true);
}