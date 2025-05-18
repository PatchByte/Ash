#include "StringObjectTest.h"
#include "Ash/AshStream.h"
#include <AshObjects/AshString.h>

static constexpr std::string_view smStringAsciiObjectTestExpectedString = "Hello, world!";
static constexpr std::wstring_view smStringWideObjectTestExpectedString = L"Hello, world!";

ash::Result StringAsciiObjectTest::Do()
{
    ash::Buffer* buffer = nullptr;

    {
        // Write
        ash::StreamExpandableExportBuffer* bufferStream = new ash::StreamExpandableExportBuffer();
        ash::objects::AshAsciiString testString = smStringAsciiObjectTestExpectedString;

        if(testString.Export(bufferStream) == false)
        {
            return ash::Result(false, "Export failed.");
        }

        buffer = bufferStream->MakeCopyOfBuffer();

        delete bufferStream;
    }
    {
        // Read
        ash::Stream* bufferStream = new ash::StreamStaticBuffer(buffer, ash::StreamMode::READ);
        ash::objects::AshAsciiString testString = "";

        if(testString.Import(bufferStream) == false)
        {
            return ash::Result(false, "Import failed.");
        }

        if(testString.GetText() != smStringAsciiObjectTestExpectedString)
        {
            return ash::Result(false, "Imported string did not match.");
        }

        delete bufferStream;
    }

    delete buffer;
    return ash::Result(true);
}

ash::Result StringWideObjectTest::Do()
{
    ash::Buffer* buffer = new ash::Buffer();

    {
        // Write
        ash::StreamExpandableExportBuffer* bufferStream = new ash::StreamExpandableExportBuffer();
        ash::objects::AshWideString testString = smStringWideObjectTestExpectedString;

        if(testString.Export(bufferStream) == false)
        {
            return ash::Result(false, "Export failed.");
        }

        buffer = bufferStream->MakeCopyOfBuffer();

        delete bufferStream;
    }
    {
        // Read
        ash::Stream* bufferStream = new ash::StreamStaticBuffer(buffer, ash::StreamMode::READ);
        ash::objects::AshWideString testString = L"";

        if(testString.Import(bufferStream) == false)
        {
            return ash::Result(false, "Import failed.");
        }

        if(testString.GetText() != smStringWideObjectTestExpectedString)
        {
            return ash::Result(false, "Imported string did not match.");
        }

        delete bufferStream;
    }

    delete buffer;
    return ash::Result(true);
}
