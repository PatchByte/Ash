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

ash::Result PointerVectorTest::Do()
{
    ash::Buffer* buffer = nullptr;
    ash::CRC32State expectedCRC = ash::CRC32State();

    // Write
    {
        ash::StreamExpandableExportBuffer* bufferStream = new ash::StreamExpandableExportBuffer();
        ash::objects::AshPointerVector<ash::objects::AshAsciiString> testVector = ash::objects::AshPointerVector<ash::objects::AshAsciiString>();

        int i = 0;

        for(ash::objects::AshAsciiString* c : smTestVector)
        {
            testVector.push_back(c);
            ++i;

            std::string cs = c->GetText();

            expectedCRC.UpdateWithPrimitive(i);
            expectedCRC.Update(cs.data(), cs.length());
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
        ash::objects::AshPointerVector<ash::objects::AshAsciiString> testVector = ash::objects::AshPointerVector<ash::objects::AshAsciiString>();
        ash::CRC32State calculatedCRC = ash::CRC32State();

        if(testVector.Import(bufferStream) == false)
        {
            return ash::Result(false, "The vector testVector import failed.");
        }

        int i = 0;

        for(auto c : testVector)
        {
            ++i;

            std::string cs = c->GetText();

            calculatedCRC.UpdateWithPrimitive(i);
            calculatedCRC.Update(cs.data(), cs.length());
        }

        if(calculatedCRC.GetValue() != expectedCRC.GetValue())
        {
            return ash::Result(false, "The calculatedCRC and expectedCRC did not match.");
        }

        delete bufferStream;
    }

    return ash::Result(true);
}
