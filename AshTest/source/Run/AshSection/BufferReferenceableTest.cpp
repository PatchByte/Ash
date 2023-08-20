#include "BufferReferenceableTest.h"
#include <Ash/AshReferenceableBuffer.h>

ash::AshResult BufferReferenceableRunTest::Do()
{
    ash::AshReferenceableBuffer* referenceableBuffer = new ash::AshReferenceableBuffer();

    if(referenceableBuffer->AllocateSize(0xFF) == false)
    {
        return ash::AshResult(false, "Allocation of referenceableBuffer failed.");
    }

    for(ash::AshSize currentIndex = 0; currentIndex < referenceableBuffer->GetSize(); currentIndex++)
    {
        referenceableBuffer->GetBytes()[currentIndex] = currentIndex;
    }

    std::vector<ash::AshReferenceableHandle*> handles = {};

    for(ash::AshSize currentIndex = 0; currentIndex < referenceableBuffer->GetSize(); currentIndex++)
    {
        ash::AshReferenceableHandle* currentIndexHandle = referenceableBuffer->ReferenceOffset(currentIndex);

        if(*currentIndexHandle->GetLocationAs<unsigned char>() != (unsigned char)currentIndex)
        {
            return ash::AshResult(false, "Expected value did not match.");
        }

        handles.push_back(currentIndexHandle);
    }

    // Now every handle should be invalidated
    delete referenceableBuffer;
    referenceableBuffer = nullptr;

    bool result = true;

    for(auto currentHandles : handles)
    {
        result &= (currentHandles->IsValid() == false);
    }

    return ash::AshResult(result);
}