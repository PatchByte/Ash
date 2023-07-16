#include "StreamExpandableTest.h"

#include <iostream>

#include <Ash/AshBuffer.h>
#include <Ash/AshStream.h>

ash::AshResult StreamExpandableTest::Do()
{
    ash::AshBuffer* copyOfStreamExpandableBuffer = nullptr;

    {
        ash::AshStreamExpandableExportBuffer* streamExpandableWriter = new ash::AshStreamExpandableExportBuffer();

        streamExpandableWriter->Write<int>(0xdead);
        streamExpandableWriter->Write<int>(0xbeef);

        copyOfStreamExpandableBuffer = streamExpandableWriter->MakeCopyOfBuffer();

        delete streamExpandableWriter;
    }
    {
        ash::AshStream* streamExpandableReader = new ash::AshStreamStaticBuffer(copyOfStreamExpandableBuffer, ash::AshStreamMode::READ);

        if(streamExpandableReader->Read<int>() != 0xdead || streamExpandableReader->Read<int>() != 0xbeef)
        {
            return ash::AshResult(false, "The tests expected values did not match.");
        }

        delete streamExpandableReader;
    }

    delete copyOfStreamExpandableBuffer;

    return ash::AshResult(true);
}