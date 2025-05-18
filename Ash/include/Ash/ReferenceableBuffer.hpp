#ifndef ASH_REFERENCEABLEBUFFER_H
#define ASH_REFERENCEABLEBUFFER_H

#include "Buffer.hpp"

namespace ash
{
    class ReferenceableHandle;
    using ReferenceableHandleId = unsigned long long;

    class ReferenceableBuffer : public Buffer
    {
    public:
        // Inherit all the constructors
        ReferenceableBuffer();
        ReferenceableBuffer(Pointer Pointer, Size Size);
        ReferenceableBuffer(ReferenceableBuffer& Source);
        ~ReferenceableBuffer();

        // AshBuffer functions.

        bool Release();

        // AshReferenceableBuffer functions.

        ReferenceableHandle* ReferenceOffset(ash::Size Offset, ash::Size Size = 1);

        inline ReferenceableHandle* ReferenceAddress(ash::Pointer Pointer, ash::Size Size = 1)
        {
            ash::Size offsetCalculated = (ash::Size)(Pointer) - (ash::Size)(this->GetPointer());

            if (offsetCalculated > this->GetSize())
            {
                return nullptr;
            }

            return this->ReferenceOffset(offsetCalculated, Size);
        }

        template <typename T>
        inline ReferenceableHandle* ReferenceTypeAddress(T* Pointer, ash::Size Size = 1)
        {
            return this->ReferenceAddress(reinterpret_cast<void*>(Pointer), Size);
        }

        std::vector<ReferenceableHandle*> FindReferencesAtOffset(ash::Size Offset, bool OnlyOffsetsDirectlyAtAddress = false);

        bool InvalidateAllReferences();
        ash::Size GetReferencesCount();

        ReferenceableBuffer& operator=(ReferenceableBuffer Source);

    private:
        virtual bool NotifyReferenceRemoval(ReferenceableHandleId ReferenceId);
        ASH_CLASS_DECLARE_FRIEND(ReferenceableHandle);

    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshReferenceableBuffer);
    };

    class ReferenceableHandle
    {
    public:
        static constexpr ReferenceableHandleId INVALID_REFERENCE_ID = (ash::Size)-1;

        ReferenceableHandle(ReferenceableHandle& Source) = delete;
        virtual ~ReferenceableHandle();

        virtual ash::Size GetId();
        virtual ash::ReferenceableBuffer* GetParent();
        virtual ash::Size GetOffset();
        virtual void* GetLocation();

        template <typename T>
        inline T* GetLocationAs()
        {
            return static_cast<T*>(this->GetLocation());
        }

        virtual ash::Size GetSize();
        virtual bool IsValid();

        ReferenceableHandle& operator=(ReferenceableHandle Source) = delete;

    private:
        ReferenceableHandle();
        void Invalidate();

        ASH_CLASS_ADD_INTERNAL_OBJECT(AshReferenceableHandle);
        ASH_CLASS_DECLARE_FRIEND(ReferenceableBuffer);
    };

} // namespace ash

#endif // !ASH_REFERENCEABLEBUFFER_H
