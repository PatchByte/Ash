#include "SerializerTest.h"

#include <iostream>

#include <Ash/AshBuffer.h>
#include <Ash/AshStream.h>
#include <Ash/AshSerializer.h>

static constexpr int testInteger1Expected = 0xFEEDBEEF;
static constexpr int testInteger2Expected = 0xDEADC0DE;

ash::Result SerializerRunTest::Do()
{
    std::string SerializedText = "";

    {
        ash::Buffer* testBuffer = new ash::Buffer();
        ash::Stream* testBufferStream = new ash::StreamStaticBuffer(testBuffer, ash::StreamMode::WRITE);

        if(testBuffer->AllocateSize(8) == false)
        {
            return ash::Result(false, "Couldn't allocate a buffer by the size of 8");
        }

        testBufferStream->Write<int>(testInteger1Expected);
        testBufferStream->Write<int>(testInteger2Expected);

        SerializedText = ash::TextSerializer::Encode(testBuffer);

        delete testBufferStream;
        delete testBuffer;
    }

    {
        ash::Buffer* testBuffer = ash::TextSerializer::Decode(SerializedText);
        ash::Stream* testBufferStream = new ash::StreamStaticBuffer(testBuffer, ash::StreamMode::READ);

        int testInteger1 = testBufferStream->Read<int>();
        int testInteger2 = testBufferStream->Read<int>();

        if(testInteger1 != testInteger1Expected)
        {
            return ash::Result(false, "The var testInteger1 didn't match with testInteger1Expected");
        }

        if(testInteger2 != testInteger2Expected)
        {
            return ash::Result(false, "The var testInteger1 didn't match with testInteger1Expected");
        }

        delete testBufferStream;
        delete testBuffer;
    }

    return ash::Result(true);
}
