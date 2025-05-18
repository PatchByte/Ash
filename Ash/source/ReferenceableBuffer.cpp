#include "Ash/ReferenceableBuffer.hpp"
#include <map>

namespace ash
{
    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshReferenceableBuffer)
    {
    public:
        ClassAshReferenceableBufferInternalObject() : m_ReferencesCount(0), m_ReferencesMap()
        {
        }

        ~ClassAshReferenceableBufferInternalObject()
        {
            this->Reset();
        }

        void Reset()
        {
            m_ReferencesCount = 0;
            m_ReferencesMap.clear();
        }

    private:
        ReferenceableHandleId m_ReferencesCount;
        std::map<ReferenceableHandleId, ReferenceableHandle*> m_ReferencesMap;

        ASH_CLASS_DECLARE_FRIEND(ReferenceableBuffer);
    };

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshReferenceableHandle)
    {
    public:
        ClassAshReferenceableHandleInternalObject() : m_ReferenceId(ReferenceableHandle::INVALID_REFERENCE_ID), m_Parent(nullptr), m_Offset(0), m_Size(0)
        {
        }

        ~ClassAshReferenceableHandleInternalObject()
        {
            this->Reset();
        }

        void Reset()
        {
            m_ReferenceId = ReferenceableHandle::INVALID_REFERENCE_ID;
            m_Parent = nullptr;
            m_Offset = 0;
            m_Size = 0;
        }

    private:
        ReferenceableHandleId m_ReferenceId;
        ReferenceableBuffer* m_Parent;
        Size m_Offset;
        Size m_Size;

        ASH_CLASS_DECLARE_FRIEND(ReferenceableBuffer);
        ASH_CLASS_DECLARE_FRIEND(ReferenceableHandle);
    };

    // AshReferenceableBuffer

    ReferenceableBuffer::ReferenceableBuffer() : Buffer::Buffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    ReferenceableBuffer::ReferenceableBuffer(Pointer Pointer, Size Size) : Buffer::Buffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableBuffer);
    }

    ReferenceableBuffer::ReferenceableBuffer(ReferenceableBuffer& Source) : Buffer::Buffer(Source)
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

        for (auto currentReferencesIterator : classInternalAshReferenceableBuffer->m_ReferencesMap)
        {
            currentReferencesIterator.second->Invalidate();
        }

        classInternalAshReferenceableBuffer->Reset();

        return result;
    }

    // AshReferenceableBuffer functions.

    ReferenceableHandle* ReferenceableBuffer::ReferenceOffset(ash::Size Offset, ash::Size Size)
    {
        ++classInternalAshReferenceableBuffer->m_ReferencesCount;

        ReferenceableHandle* referenceHandle = new ReferenceableHandle();
        ReferenceableHandleId referenceHandleId = classInternalAshReferenceableBuffer->m_ReferencesCount;

        referenceHandle->classInternalAshReferenceableHandle->m_Offset = Offset;
        referenceHandle->classInternalAshReferenceableHandle->m_Size = Size;
        referenceHandle->classInternalAshReferenceableHandle->m_Parent = this;
        referenceHandle->classInternalAshReferenceableHandle->m_ReferenceId = referenceHandleId;

        if (classInternalAshReferenceableBuffer->m_ReferencesMap.count(referenceHandleId) > 0)
        {
            delete referenceHandle;
            return nullptr;
        }

        classInternalAshReferenceableBuffer->m_ReferencesMap.emplace(referenceHandleId, referenceHandle);

        return referenceHandle;
    }

    std::vector<ReferenceableHandle*> ReferenceableBuffer::FindReferencesAtOffset(ash::Size Offset, bool OnlyOffsetsDirectlyAtAddress)
    {
        std::vector<ReferenceableHandle*> results = {};

        for (auto currentReferenceableHandleIterator : classInternalAshReferenceableBuffer->m_ReferencesMap)
        {
            ReferenceableHandle* currentReferenceableHandle = currentReferenceableHandleIterator.second;
            ash::Size startAddress = currentReferenceableHandle->GetOffset();
            ash::Size endAddress = currentReferenceableHandle->GetOffset() + currentReferenceableHandle->GetSize();

            if (OnlyOffsetsDirectlyAtAddress)
            {
                if (Offset == startAddress)
                {
                    results.push_back(currentReferenceableHandle);
                }
            }
            else
            {
                if (Offset >= startAddress && Offset <= endAddress)
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

        for (auto currentReferenceableHandleIterator : classInternalAshReferenceableBuffer->m_ReferencesMap)
        {
            references.push_back(currentReferenceableHandleIterator.second);
        }

        for (auto currentReferenceableHandleIterator : references)
        {
            this->NotifyReferenceRemoval(currentReferenceableHandleIterator->GetId());
            currentReferenceableHandleIterator->Invalidate();
        }

        return true;
    }

    ash::Size ReferenceableBuffer::GetReferencesCount()
    {
        return classInternalAshReferenceableBuffer->m_ReferencesMap.size();
    }

    ReferenceableBuffer& ReferenceableBuffer::operator=(ReferenceableBuffer Source)
    {
        Buffer::operator=(Source);

        // The Buffer wouldn't know where to assign the existing references.
        classInternalAshReferenceableBuffer->m_ReferencesCount = 0;
        classInternalAshReferenceableBuffer->m_ReferencesMap = {};

        return *this;
    }

    bool ReferenceableBuffer::NotifyReferenceRemoval(ReferenceableHandleId ReferenceId)
    {
        if (classInternalAshReferenceableBuffer->m_ReferencesMap.count(ReferenceId) <= 0)
        {
            return false;
        }

        classInternalAshReferenceableBuffer->m_ReferencesMap.erase(ReferenceId);

        return true;
    }

    // AshReferenceableHandle

    ReferenceableHandle::ReferenceableHandle()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshReferenceableHandle);
    }

    ReferenceableHandle::~ReferenceableHandle()
    {
        if (this->IsValid())
        {
            classInternalAshReferenceableHandle->m_Parent->NotifyReferenceRemoval(classInternalAshReferenceableHandle->m_ReferenceId);
        }

        classInternalAshReferenceableHandle->Reset();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshReferenceableHandle);
    }

    ash::Size ReferenceableHandle::GetId()
    {
        return classInternalAshReferenceableHandle->m_ReferenceId;
    }

    ash::ReferenceableBuffer* ReferenceableHandle::GetParent()
    {
        return classInternalAshReferenceableHandle->m_Parent;
    }

    ash::Size ReferenceableHandle::GetOffset()
    {
        if (this->IsValid() == false)
        {
            return 0;
        }

        return classInternalAshReferenceableHandle->m_Offset;
    }

    void* ReferenceableHandle::GetLocation()
    {
        if (this->IsValid() == false)
        {
            return nullptr;
        }

        return static_cast<void*>(classInternalAshReferenceableHandle->m_Parent->GetBytes() + classInternalAshReferenceableHandle->m_Offset);
    }

    ash::Size ReferenceableHandle::GetSize()
    {
        if (this->IsValid() == false)
        {
            return 0;
        }

        return classInternalAshReferenceableHandle->m_Size;
    }

    bool ReferenceableHandle::IsValid()
    {
        if (classInternalAshReferenceableHandle->m_Parent == nullptr)
        {
            return false;
        }
        if (classInternalAshReferenceableHandle->m_ReferenceId == ReferenceableHandle::INVALID_REFERENCE_ID ||
            classInternalAshReferenceableHandle->m_Offset >= classInternalAshReferenceableHandle->m_Parent->GetSize() ||
            (classInternalAshReferenceableHandle->m_Offset + classInternalAshReferenceableHandle->m_Size) > classInternalAshReferenceableHandle->m_Parent->GetSize())
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
} // namespace ash
