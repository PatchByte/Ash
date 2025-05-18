#include "StreamExpandableTest.h"

#include <iostream>

#include <Ash/AshBuffer.h>
#include <Ash/AshStream.h>

ash::Result StreamExpandableTest::Do()
{
    ash::Buffer* copyOfStreamExpandableBuffer = nullptr;

    {
        ash::StreamExpandableExportBuffer* streamExpandableWriter = new ash::StreamExpandableExportBuffer();

        streamExpandableWriter->Write<int>(0xdead);
        streamExpandableWriter->Write<int>(0xbeef);

        copyOfStreamExpandableBuffer = streamExpandableWriter->MakeCopyOfBuffer();

        delete streamExpandableWriter;
    }
    {
        ash::Stream* streamExpandableReader = new ash::StreamStaticBuffer(copyOfStreamExpandableBuffer, ash::StreamMode::READ);

        if(streamExpandableReader->Read<int>() != 0xdead || streamExpandableReader->Read<int>() != 0xbeef)
        {
            return ash::Result(false, "The tests expected values did not match.");
        }

        delete streamExpandableReader;
    }

    delete copyOfStreamExpandableBuffer;

    return ash::Result(true);
}
