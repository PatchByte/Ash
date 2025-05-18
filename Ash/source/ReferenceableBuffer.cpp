#include "Ash/ReferenceableBuffer.h"
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
        ReferenceableHandleId referencesCount;
        std::map<ReferenceableHandleId, ReferenceableHandle*> referencesMap;

        ASH_CLASS_DECLARE_FRIEND(ReferenceableBuffer);
    };

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshReferenceableHandle)
    {
    public:
        ClassAshReferenceableHandleInternalObject():
            referenceId(ReferenceableHandle::INVALID_REFERENCE_ID),
            parent(nullptr),
            offset(0),
            size(0)
        {}

        ~ClassAshReferenceableHandleInternalObject()
        {
            this->Reset();
        }

        void Reset()
        {
            referenceId = ReferenceableHandle::INVALID_REFERENCE_ID;
            parent = nullptr;
            offset = 0;
            size = 0;
        }
    private:
        ReferenceableHandleId referenceId;
        ReferenceableBuffer* parent;
        Size offset;
        Size size;

        ASH_CLASS_DECLARE_FRIEND(ReferenceableBuffer);
        ASH_CLASS_DECLARE_FRIEND(ReferenceableHandle);
    };

    // AshReferenceableBuffer

    ReferenceableBuffer::ReferenceableBuffer():
        Buffer::Buffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    ReferenceableBuffer::ReferenceableBuffer(Pointer Pointer, Size Size):
        Buffer::Buffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    ReferenceableBuffer::ReferenceableBuffer(ReferenceableBuffer& Source):
        Buffer::Buffer(Source)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    ReferenceableBuffer::~ReferenceableBuffer()
    {
        this->Release();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshReferenceableBuffer);
    }

    // AshBuffer functions.

    bool ReferenceableBuffer::Release()
    {
        bool result = Buffer::Release();

        for(auto currentReferencesIterator : classInternalAshReferenceableBuffer->referencesMap)
        {
            currentReferencesIterator.second->Invalidate();
        }

        classInternalAshReferenceableBuffer->Reset();

        return result;
    }

    // AshReferenceableBuffer functions.

    ReferenceableHandle* ReferenceableBuffer::ReferenceOffset(ash::Size Offset, ash::Size Size)
    {
        ++classInternalAshReferenceableBuffer->referencesCount;

        ReferenceableHandle* referenceHandle = new ReferenceableHandle();
        ReferenceableHandleId referenceHandleId = classInternalAshReferenceableBuffer->referencesCount;

        referenceHandle->classInternalAshReferenceableHandle->offset = Offset;
        referenceHandle->classInternalAshReferenceableHandle->size = Size;
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

    std::vector<ReferenceableHandle*> ReferenceableBuffer::FindReferencesAtOffset(ash::Size Offset, bool OnlyOffsetsDirectlyAtAddress)
    {
        std::vector<ReferenceableHandle*> results = {};

        for(auto currentReferenceableHandleIterator : classInternalAshReferenceableBuffer->referencesMap)
        {
            ReferenceableHandle* currentReferenceableHandle = currentReferenceableHandleIterator.second;
            ash::Size startAddress = currentReferenceableHandle->GetOffset();
            ash::Size endAddress = currentReferenceableHandle->GetOffset() + currentReferenceableHandle->GetSize();

            if(OnlyOffsetsDirectlyAtAddress)
            {
                if(Offset == startAddress)
                {
                    results.push_back(currentReferenceableHandle);
                }
            }
            else
            {
                if(
                    Offset >= startAddress &&
                    Offset <= endAddress
                )
                {
                    results.push_back(currentReferenceableHandle);
                }
            }
        }

        return results;
    }

    bool ReferenceableBuffer::InvalidateAllReferences()
    {
        std::vector<ReferenceableHandle*> references = {};

        for(auto currentReferenceableHandleIterator : classInternalAshReferenceableBuffer->referencesMap)
        {
            references.push_back(currentReferenceableHandleIterator.second);
        }

        for(auto currentReferenceableHandleIterator : references)
        {
            this->NotifyReferenceRemoval(currentReferenceableHandleIterator->GetId());
            currentReferenceableHandleIterator->Invalidate();
        }

        return true;
    }

    ash::Size ReferenceableBuffer::GetReferencesCount()
    {
        return classInternalAshReferenceableBuffer->referencesMap.size();
    }

    ReferenceableBuffer& ReferenceableBuffer::operator=(ReferenceableBuffer Source)
    {
        Buffer::operator=(Source);

        // The Buffer wouldn't know where to assign the existing references.
        classInternalAshReferenceableBuffer->referencesCount = 0;
        classInternalAshReferenceableBuffer->referencesMap = {};

        return *this;
    }

    bool ReferenceableBuffer::NotifyReferenceRemoval(ReferenceableHandleId ReferenceId)
    {
        if(classInternalAshReferenceableBuffer->referencesMap.count(ReferenceId) <= 0)
        {
            return false;
        }

        classInternalAshReferenceableBuffer->referencesMap.erase(ReferenceId);

        return true;
    }

    // AshReferenceableHandle

    ReferenceableHandle::ReferenceableHandle()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableHandle);
    }

    ReferenceableHandle::~ReferenceableHandle()
    {
        if(this->IsValid())
        {
            classInternalAshReferenceableHandle->parent->NotifyReferenceRemoval(classInternalAshReferenceableHandle->referenceId);
        }

        classInternalAshReferenceableHandle->Reset();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshReferenceableHandle);
    }

    ash::Size ReferenceableHandle::GetId()
    {
        return classInternalAshReferenceableHandle->referenceId;
    }

    ash::ReferenceableBuffer* ReferenceableHandle::GetParent()
    {
        return classInternalAshReferenceableHandle->parent;
    }

    ash::Size ReferenceableHandle::GetOffset()
    {
        if(this->IsValid() == false)
        {
            return 0;
        }

        return classInternalAshReferenceableHandle->offset;
    }

    void* ReferenceableHandle::GetLocation()
    {
        if(this->IsValid() == false)
        {
            return nullptr;
        }

        return static_cast<void*>(classInternalAshReferenceableHandle->parent->GetBytes() + classInternalAshReferenceableHandle->offset);
    }

    ash::Size ReferenceableHandle::GetSize()
    {
        if(this->IsValid() == false)
        {
            return 0;
        }

        return classInternalAshReferenceableHandle->size;
    }

    bool ReferenceableHandle::IsValid()
    {
        if(classInternalAshReferenceableHandle->parent == nullptr) { return false; }
        if(
            classInternalAshReferenceableHandle->referenceId == ReferenceableHandle::INVALID_REFERENCE_ID ||
            classInternalAshReferenceableHandle->offset >= classInternalAshReferenceableHandle->parent->GetSize() ||
            (classInternalAshReferenceableHandle->offset + classInternalAshReferenceableHandle->size) > classInternalAshReferenceableHandle->parent->GetSize()
        )
        {
            return false;
        }

        return true;
    }

    void ReferenceableHandle::Invalidate()
    {
        // Should be good enough
        classInternalAshReferenceableHandle->Reset();
    }
}
