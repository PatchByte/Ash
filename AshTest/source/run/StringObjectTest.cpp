#include "StringObjectTest.h"
#include <AshObjects/AshString.h>

static constexpr std::string_view smStringObjectTestExpectedString = "Hello, world!";

ash::AshResult StringObjectTest::Do()
{
    ash::AshBuffer* buffer = new ash::AshBuffer();

    {
        // Write
        ash::AshStream* bufferStream = new ash::AshStream(buffer, ash::AshStreamMode::WRITE);
        ash::objects::AshString testString = smStringObjectTestExpectedString;

        buffer->AllocateSize(testString.GetExportSize());

        if(testString.Export(bufferStream) == false)
        {
            return ash::AshResult(false, "Export failed.");
        }

        delete bufferStream;
    }
    {
        // Read
        ash::AshStream* bufferStream = new ash::AshStream(buffer, ash::AshStreamMode::READ);
        ash::objects::AshString testString = "";

        if(testString.Import(bufferStream) == false)
        {
            return ash::AshResult(false, "Import failed.");
        }

        if(testString.GetText() != smStringObjectTestExpectedString)
        {
            return ash::AshResult(false, "Imported string did not match.");
        }

        delete bufferStream;
    }

    delete buffer;
    return ash::AshResult(true);
}