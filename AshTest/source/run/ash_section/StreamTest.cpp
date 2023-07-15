#include "StreamTest.h"
#include <Ash/AshStream.h>

#include <iostream>
#include <sstream>

ash::AshResult StreamRunTest::Do()
{
    unsigned char blob[64 * sizeof(int)] = { 0 };

    {
        // Write test

        ash::AshStreamStaticBuffer* stream = new ash::AshStreamStaticBuffer(new ash::AshBuffer(blob, sizeof(blob)), ash::AshStreamMode::WRITE);
        stream->DeallocateBufferAfterUse();

        for(int i = 0; i < 64; i++)
        {
            stream->Write(&i);
        }
        
        if(stream->IsEndOfStream() == false)
        {
            return ash::AshResult(false, "Stream is not eof.");
        }

        delete stream;
    }
    {
        // Read Test

        ash::AshStreamStaticBuffer* stream = new ash::AshStreamStaticBuffer(new ash::AshBuffer(blob, sizeof(blob)), ash::AshStreamMode::READ);
        stream->DeallocateBufferAfterUse();

        for(int i = 0; i < 64; i++)
        {
            int a = stream->Read<int>();
            if(a != i)
            {
                return ash::AshResult(false, "Mismatch.");
            }
        }

        delete stream;
    }

    return ash::AshResult(true);
}