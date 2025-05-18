#include "StreamTest.h"
#include <Ash/AshStream.h>

#include <iostream>
#include <sstream>

ash::Result StreamRunTest::Do()
{
    unsigned char blob[64 * sizeof(int)] = { 0 };

    {
        // Write test

        ash::StreamStaticBuffer* stream = new ash::StreamStaticBuffer(new ash::Buffer(blob, sizeof(blob)), ash::StreamMode::WRITE);
        stream->DeallocateBufferAfterUse();

        for(int i = 0; i < 64; i++)
        {
            stream->WriteFromPointer(&i);
        }

        if(stream->IsEndOfStream() == false)
        {
            return ash::Result(false, "Stream is not eof.");
        }

        delete stream;
    }
    {
        // Read Test

        ash::StreamStaticBuffer* stream = new ash::StreamStaticBuffer(new ash::Buffer(blob, sizeof(blob)), ash::StreamMode::READ);
        stream->DeallocateBufferAfterUse();

        for(int i = 0; i < 64; i++)
        {
            int a = stream->Read<int>();
            if(a != i)
            {
                return ash::Result(false, "Mismatch.");
            }
        }

        delete stream;
    }

    return ash::Result(true);
}
