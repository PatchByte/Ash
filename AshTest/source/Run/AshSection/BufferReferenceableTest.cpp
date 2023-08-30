#include "BufferReferenceableTest.h"
#include <Ash/AshReferenceableBuffer.h>
#include <algorithm>

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

    {
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
        referenceableBuffer->InvalidateAllReferences();

        bool result = true;

        for(auto currentHandle : handles)
        {
            result &= (currentHandle->IsValid() == false);
            delete currentHandle;
        }

        if(result == false)
        {
            return ash::AshResult(result, "Handles sanity check failed.");
        }
    }

    {
        ash::AshReferenceableHandle* handle01_04_1 = referenceableBuffer->ReferenceOffset(0x01, 0x03);
        ash::AshReferenceableHandle* handle01_04_2 = referenceableBuffer->ReferenceOffset(0x02, 0x02);
        ash::AshReferenceableHandle* handle01_04_3 = referenceableBuffer->ReferenceOffset(0x03, 0x01);
        ash::AshReferenceableHandle* handle01_04_4 = referenceableBuffer->ReferenceOffset(0x04, 0x04);
        ash::AshReferenceableHandle* handle01_04_4_dup = referenceableBuffer->ReferenceOffset(0x04, 0x01);
        ash::AshReferenceableHandle* handle01_04_5 = referenceableBuffer->ReferenceOffset(0x05, 0x04);

        auto refs = referenceableBuffer->FindReferencesAtOffset(0x03, false);
        
        bool sanityCheck = (std::find(refs.begin(), refs.end(), handle01_04_1) != refs.end()) && 
                           (std::find(refs.begin(), refs.end(), handle01_04_2) != refs.end()) && 
                           (std::find(refs.begin(), refs.end(), handle01_04_3) != refs.end());

        refs = referenceableBuffer->FindReferencesAtOffset(0x05, false);
        
        sanityCheck &=  (std::find(refs.begin(), refs.end(), handle01_04_4) != refs.end()) &&
                        (std::find(refs.begin(), refs.end(), handle01_04_5) != refs.end());
        
        // Sanity test to check if it received to notification of a removal.
        delete handle01_04_1;

        refs = referenceableBuffer->FindReferencesAtOffset(0x04, true);

        sanityCheck &=  (std::find(refs.begin(), refs.end(), handle01_04_4) != refs.end()) &&
                        (std::find(refs.begin(), refs.end(), handle01_04_4_dup) != refs.end());
        
        sanityCheck &= (refs.size() == 2);

        if(sanityCheck == false)
        {
            return ash::AshResult(false, "AshReferenceableHandle location checks failed.");
        }
    }

    return ash::AshResult(true);
}