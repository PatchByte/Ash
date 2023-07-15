#include "StringObjectTest.h"
#include <AshObjects/AshString.h>

static constexpr std::string_view smStringAsciiObjectTestExpectedString = "Hello, world!";
static constexpr std::wstring_view smStringWideObjectTestExpectedString = L"Hello, world!";

ash::AshResult StringAsciiObjectTest::Do()
{
    ash::AshBuffer* buffer = new ash::AshBuffer();

    {
        // Write
        ash::AshStream* bufferStream = new ash::AshStreamStaticBuffer(buffer, ash::AshStreamMode::WRITE);
        ash::objects::AshAsciiString testString = smStringAsciiObjectTestExpectedString;

        buffer->AllocateSize(testString.GetExportSize());

        if(testString.Export(bufferStream) == false)
        {
            return ash::AshResult(false, "Export failed.");
        }

        delete bufferStream;
    }
    {
        // Read
        ash::AshStream* bufferStream = new ash::AshStreamStaticBuffer(buffer, ash::AshStreamMode::READ);
        ash::objects::AshAsciiString testString = "";

        if(testString.Import(bufferStream) == false)
        {
            return ash::AshResult(false, "Import failed.");
        }

        if(testString.GetText() != smStringAsciiObjectTestExpectedString)
        {
            return ash::AshResult(false, "Imported string did not match.");
        }

        delete bufferStream;
    }

    delete buffer;
    return ash::AshResult(true);
}

ash::AshResult StringWideObjectTest::Do()
{
    ash::AshBuffer* buffer = new ash::AshBuffer();

    {
        // Write
        ash::AshStream* bufferStream = new ash::AshStreamStaticBuffer(buffer, ash::AshStreamMode::WRITE);
        ash::objects::AshWideString testString = smStringWideObjectTestExpectedString;

        buffer->AllocateSize(testString.GetExportSize());

        if(testString.Export(bufferStream) == false)
        {
            return ash::AshResult(false, "Export failed.");
        }

        delete bufferStream;
    }
    {
        // Read
        ash::AshStream* bufferStream = new ash::AshStreamStaticBuffer(buffer, ash::AshStreamMode::READ);
        ash::objects::AshWideString testString = L"";

        if(testString.Import(bufferStream) == false)
        {
            return ash::AshResult(false, "Import failed.");
        }

        if(testString.GetText() != smStringWideObjectTestExpectedString)
        {
            return ash::AshResult(false, "Imported string did not match.");
        }

        delete bufferStream;
    }

    delete buffer;
    return ash::AshResult(true);
}