#include "SerializerTest.h"

#include <iostream>

#include <Ash/AshBuffer.h>
#include <Ash/AshStream.h>
#include <Ash/AshSerializer.h>

static constexpr int testInteger1Expected = 0xFEEDBEEF;
static constexpr int testInteger2Expected = 0xDEADC0DE;

ash::AshResult SerializerRunTest::Do()
{
    std::string SerializedText = "";

    {
        ash::AshBuffer* testBuffer = new ash::AshBuffer();
        ash::AshStream* testBufferStream = new ash::AshStreamStaticBuffer(testBuffer, ash::AshStreamMode::WRITE);

        if(testBuffer->AllocateSize(8) == false)
        {
            return ash::AshResult(false, "Couldn't allocate a buffer by the size of 8");
        }

        testBufferStream->Write<int>(testInteger1Expected);
        testBufferStream->Write<int>(testInteger2Expected);

        SerializedText = ash::AshSerializer::Encode(testBuffer);

        delete testBufferStream;
        delete testBuffer;
    }

    {
        ash::AshBuffer* testBuffer = ash::AshSerializer::Decode(SerializedText);
        ash::AshStream* testBufferStream = new ash::AshStreamStaticBuffer(testBuffer, ash::AshStreamMode::READ);

        int testInteger1 = testBufferStream->Read<int>();
        int testInteger2 = testBufferStream->Read<int>();

        if(testInteger1 != testInteger1Expected)
        {
            return ash::AshResult(false, "The var testInteger1 didn't match with testInteger1Expected");
        }

        if(testInteger2 != testInteger2Expected)
        {
            return ash::AshResult(false, "The var testInteger1 didn't match with testInteger1Expected");
        }

        delete testBufferStream;
        delete testBuffer;
    }

    return ash::AshResult(true);
}