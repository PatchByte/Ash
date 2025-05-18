#include "BufferTest.h"
#include "Ash/AshBuffer.h"

ash::Result BufferRunTest::Do()
{
    {
        // Import test
        constexpr int expectedVar = 10;
        int var = 10;

        ash::Buffer* testBuffer = new ash::Buffer();

        if(testBuffer->ImportPointer(&var, sizeof(var)) == false)
        {
            delete testBuffer;
            return ash::Result(false, "ImportPointer failed.");
        }

        if(*testBuffer->GetBuffer<int>() != expectedVar)
        {
            delete testBuffer;
            return ash::Result(false, "ImportPointer variables did not match.");
        }

        delete testBuffer;
    }
    {
        // AllocateSize test
        constexpr int expectedVar = 20;
        int var = 20;

        ash::Buffer* testBuffer = new ash::Buffer();

        if(testBuffer->AllocateSize(sizeof(int)) == false)
        {
            delete testBuffer;
            return ash::Result(false, "AllocateSize failed.");
        }

        *testBuffer->GetBuffer<int>() = expectedVar;

        if(*testBuffer->GetBuffer<int>() != expectedVar)
        {
            delete testBuffer;
            return ash::Result(false, "AllocateSize variables did not match.");
        }

        delete testBuffer;
    }

    return ash::Result(true);
}
