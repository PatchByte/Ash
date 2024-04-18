#include "PrimitiveTest.h"
#include "Ash/AshStream.h"

#include <AshObjects/AshPrimitive.h>

static constexpr double smDoubleValueExpected = 9.F;

ash::AshResult PrimitiveTest::Do()
{
    ash::AshBuffer* buffer = nullptr;

    // Write
    {
        ash::AshStreamExpandableExportBuffer* bufferStream = new ash::AshStreamExpandableExportBuffer();
        ash::objects::AshPrimitive<double> doublePrimitive = ash::objects::AshPrimitive<double>(smDoubleValueExpected);

        if(doublePrimitive.Export(bufferStream) == false)
        {
            return ash::AshResult(false, "AshPrimitive (double) failed to export.");
        }

        buffer = bufferStream->MakeCopyOfBuffer();

        delete bufferStream;
    }

    // Read
    {
        ash::AshStream* bufferStream = new ash::AshStreamStaticBuffer(buffer, ash::AshStreamMode::READ);
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
