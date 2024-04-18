#include "PointerVectorTest.h"
#include "Ash/AshStream.h"

#include <Ash/AshCRC32.h>
#include <AshObjects/AshPointerVector.h>
#include <AshObjects/AshPrimitive.h>
#include <AshObjects/AshString.h>

#include <iostream>

static std::vector<ash::objects::AshAsciiString*> smTestVector = {
        new ash::objects::AshAsciiString("1"),
        new ash::objects::AshAsciiString("2"),
        new ash::objects::AshAsciiString("4"),
        new ash::objects::AshAsciiString("8"),
        new ash::objects::AshAsciiString("16"),
        new ash::objects::AshAsciiString("32"),
        new ash::objects::AshAsciiString("64"),
};

ash::AshResult PointerVectorTest::Do()
{
    ash::AshBuffer* buffer = nullptr;
    ash::AshCRC32 expectedCRC = 0;

    // Write
    {
        ash::AshStreamExpandableExportBuffer* bufferStream = new ash::AshStreamExpandableExportBuffer();
        ash::objects::AshPointerVector<ash::objects::AshAsciiString> testVector = ash::objects::AshPointerVector<ash::objects::AshAsciiString>();

        int i = 0;

        for(ash::objects::AshAsciiString* c : smTestVector)
        {
            testVector.push_back(c);
            ++i;

            std::string cs = c->GetText();

            expectedCRC.UpdateWithTemplateValue(i);
            expectedCRC.Update(cs.data(), cs.length());
        }

        if(testVector.Export(bufferStream) == false)
        {
            return ash::AshResult(false, "The vector testVector export failed.");
        }

        buffer = bufferStream->MakeCopyOfBuffer();

        delete bufferStream;
    }

    // Read
    {
        ash::AshStream* bufferStream = new ash::AshStreamStaticBuffer(buffer, ash::AshStreamMode::READ);
        ash::objects::AshPointerVector<ash::objects::AshAsciiString> testVector = ash::objects::AshPointerVector<ash::objects::AshAsciiString>();
        ash::AshCRC32 calculatedCRC = ash::AshCRC32();

        if(testVector.Import(bufferStream) == false)
        {
            return ash::AshResult(false, "The vector testVector import failed.");
        }

        int i = 0;

        for(auto c : testVector)
        {
            ++i;

            std::string cs = c->GetText();

            calculatedCRC.UpdateWithTemplateValue(i);
            calculatedCRC.Update(cs.data(), cs.length());
        }

        if(calculatedCRC.GetValue() != expectedCRC.GetValue())
        {
            return ash::AshResult(false, "The calculatedCRC and expectedCRC did not match.");
        }

        delete bufferStream;
    }

    return ash::AshResult(true);
}
