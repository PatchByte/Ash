#include "BufferTest.h"
#include "Ash/AshBuffer.h"

ash::AshResult BufferRunTest::Do()
{
    {
        // Import test
        constexpr int expectedVar = 10;
        int var = 10;

        ash::AshBuffer* testBuffer = new ash::AshBuffer();

        if(testBuffer->ImportPointer(&var, sizeof(var)) == false)
        {
            delete testBuffer;
            return ash::AshResult(false, "ImportPointer failed.");
        }

        if(*testBuffer->GetBuffer<int>() != expectedVar)
        {
            delete testBuffer;
            return ash::AshResult(false, "ImportPointer variables did not match.");
        }

        delete testBuffer;
    }
    {
        // AllocateSize test
        constexpr int expectedVar = 20;
        int var = 20;

        ash::AshBuffer* testBuffer = new ash::AshBuffer();

        if(testBuffer->AllocateSize(sizeof(int)) == false)
        {
            delete testBuffer;
            return ash::AshResult(false, "AllocateSize failed.");
        }

        *testBuffer->GetBuffer<int>() = expectedVar;

        if(*testBuffer->GetBuffer<int>() != expectedVar)
        {
            delete testBuffer;
            return ash::AshResult(false, "AllocateSize variables did not match.");
        }

        delete testBuffer;
    }

    return ash::AshResult(true);
}