#include "StreamTest.h"
#include <Ash/AshStream.h>

#include <iostream>
#include <sstream>

ash::AshResult StreamRunTest::Do()
{
    unsigned char blob[64 * sizeof(int)] = { 0 };

    {
        // Write test

        ash::AshBuffer* buffer = new ash::AshBuffer();
        buffer->ImportPointer(blob, sizeof(blob));

        ash::AshStream* stream = new ash::AshStream(buffer, ash::AshStreamMode::WRITE);

        for(int i = 0; i < 64; i++)
        {
            stream->Write(&i);
        }

        delete buffer;
        delete stream;
    }
    {
        // Read Test

        ash::AshBuffer* buffer = new ash::AshBuffer();
        buffer->ImportPointer(blob, sizeof(blob));

        ash::AshStream* stream = new ash::AshStream(buffer, ash::AshStreamMode::READ);

        for(int i = 0; i < 64; i++)
        {
            int a = stream->Read<int>();
            if(a != i)
            {
                return ash::AshResult(false, "Missmatch.");
            }
        }

        delete buffer;
        delete stream;
    }

    return ash::AshResult(true);
}