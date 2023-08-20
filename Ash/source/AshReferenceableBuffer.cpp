#include "Ash/AshReferenceableBuffer.h"
#include <map>

namespace ash
{
    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshReferenceableBuffer)
    {
    public:
        ClassAshReferenceableBufferInternalObject():
            referencesCount(0),
            referencesMap()
        {}

        ~ClassAshReferenceableBufferInternalObject()
        {
            this->Reset();
        }

        void Reset()
        {
            referencesCount = 0;
            referencesMap.clear();
        }
    private:
        AshReferenceableHandleId referencesCount;
        std::map<AshReferenceableHandleId, AshReferenceableHandle*> referencesMap;

        ASH_CLASS_DECLARE_FRIEND(AshReferenceableBuffer);
    };

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshReferenceableHandle)
    {
    public:
        ClassAshReferenceableHandleInternalObject():
            referenceId(AshReferenceableHandle::INVALID_REFERENCE_ID),
            parent(nullptr),
            offset(0)
        {}

        ~ClassAshReferenceableHandleInternalObject()
        {
            this->Reset();
        }

        void Reset()
        {
            referenceId = 0;
            parent = nullptr;
            offset = 0;
        }
    private:
        AshReferenceableHandleId referenceId;
        AshReferenceableBuffer* parent;
        AshSize offset;

        ASH_CLASS_DECLARE_FRIEND(AshReferenceableBuffer);
        ASH_CLASS_DECLARE_FRIEND(AshReferenceableHandle);
    };

    // AshReferenceableBuffer

    AshReferenceableBuffer::AshReferenceableBuffer():
        AshBuffer::AshBuffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    AshReferenceableBuffer::AshReferenceableBuffer(AshPointer Pointer, AshSize Size):
        AshBuffer::AshBuffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    AshReferenceableBuffer::AshReferenceableBuffer(AshReferenceableBuffer& Source):
        AshBuffer::AshBuffer(Source)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    AshReferenceableBuffer::~AshReferenceableBuffer()
    {
        classInternalAshReferenceableBuffer->Reset();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshReferenceableBuffer);
    }

    // AshBuffer functions.

    bool AshReferenceableBuffer::ReleaseMemory()
    {
        bool result = AshBuffer::ReleaseMemory();

        for(auto currentReferencesIterator : classInternalAshReferenceableBuffer->referencesMap)
        {
            currentReferencesIterator.second->Invalidate();
        }

        classInternalAshReferenceableBuffer->Reset();

        return result;
    }

    // AshReferenceableBuffer functions.

    AshReferenceableHandle* AshReferenceableBuffer::ReferenceOffset(ash::AshSize Offset)
    {
        AshReferenceableHandle* referenceHandle = new AshReferenceableHandle();
        AshReferenceableHandleId referenceHandleId = (++classInternalAshReferenceableBuffer->referencesCount);

        referenceHandle->classInternalAshReferenceableHandle->offset = Offset;
        referenceHandle->classInternalAshReferenceableHandle->parent = this;
        referenceHandle->classInternalAshReferenceableHandle->referenceId = referenceHandleId;

        if(classInternalAshReferenceableBuffer->referencesMap.count(referenceHandleId) > 0)
        {
            delete referenceHandle;
            return nullptr;
        }

        classInternalAshReferenceableBuffer->referencesMap.emplace(referenceHandleId, referenceHandle);

        return referenceHandle;
    }

    AshReferenceableBuffer& AshReferenceableBuffer::operator= (AshReferenceableBuffer Source)
    {
        AshBuffer::operator=(Source);
        // The Buffer wouldn't know where to assign the existing references.
        classInternalAshReferenceableBuffer->referencesCount = 0;
        return *this;
    }

    // AshReferenceableHandle

    AshReferenceableHandle::AshReferenceableHandle()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableHandle);
    }

    AshReferenceableHandle::~AshReferenceableHandle()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshReferenceableHandle);
    }
    
    ash::AshSize AshReferenceableHandle::GetId()
    {
        return classInternalAshReferenceableHandle->referenceId;
    }

    ash::AshReferenceableBuffer* AshReferenceableHandle::GetParent()
    {
        return classInternalAshReferenceableHandle->parent;
    }

    ash::AshSize AshReferenceableHandle::GetOffset()
    {
        if(this->IsValid() == false)
        {
            return 0;
        }

        return classInternalAshReferenceableHandle->offset;
    }

    void* AshReferenceableHandle::GetLocation()
    {
        if(this->IsValid() == false)
        {
            return nullptr;
        }

        return static_cast<void*>(classInternalAshReferenceableHandle->parent->GetBytes() + classInternalAshReferenceableHandle->offset);
    }

    bool AshReferenceableHandle::IsValid()
    {
        if(classInternalAshReferenceableHandle->parent == nullptr) { return false; }
        if(classInternalAshReferenceableHandle->referenceId == AshReferenceableHandle::INVALID_REFERENCE_ID ||
           classInternalAshReferenceableHandle->offset > classInternalAshReferenceableHandle->parent->GetSize())
        {
            return false;
        }

        return true;
    }
}