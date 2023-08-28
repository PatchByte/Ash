#pragma once
#include "AshBuffer.h"

namespace ash
{
    class AshReferenceableHandle;
    using AshReferenceableHandleId = unsigned long long;
    
    class AshReferenceableBuffer : public AshBuffer
    {
    public:
        // Inherit all the constructors
        AshReferenceableBuffer();
        AshReferenceableBuffer(AshPointer Pointer, AshSize Size);
        AshReferenceableBuffer(AshReferenceableBuffer& Source);
        ~AshReferenceableBuffer();

        // AshBuffer functions.

        bool ReleaseMemory();

        // AshReferenceableBuffer functions.

        AshReferenceableHandle* ReferenceOffset(ash::AshSize Offset, ash::AshSize Size = 1);

        inline AshReferenceableHandle* ReferenceAddress(ash::AshPointer Pointer, ash::AshSize Size = 1)
        {
            ash::AshSize offsetCalculated = (ash::AshSize)(Pointer) - (ash::AshSize)(this->GetPointer());

            if(offsetCalculated > this->GetSize())
            {
                return nullptr;
            }

            return this->ReferenceOffset(offsetCalculated, Size);
        }

        template<typename T> 
        inline AshReferenceableHandle* ReferenceTypeAddress(T* Pointer, ash::AshSize Size = 1)
        {
            return this->ReferenceAddress(reinterpret_cast<void*>(Pointer), Size);
        }

        std::vector<AshReferenceableHandle*> FindReferencesAtOffset(ash::AshSize Offset, bool OnlyOffsetsDirectlyAtAddress = false);

        ash::AshSize GetReferencesCount();

        AshReferenceableBuffer& operator=(AshReferenceableBuffer Source);
    private:
        virtual bool NotifyReferenceRemoval(AshReferenceableHandleId ReferenceId);
        ASH_CLASS_DECLARE_FRIEND(AshReferenceableHandle);
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshReferenceableBuffer);
    };

    class AshReferenceableHandle
    {
    public:
        static constexpr AshReferenceableHandleId INVALID_REFERENCE_ID = (ash::AshSize)-1;

        AshReferenceableHandle(AshReferenceableHandle& Source) = delete;
        virtual ~AshReferenceableHandle();

        virtual ash::AshSize GetId();
        virtual ash::AshReferenceableBuffer* GetParent();
        virtual ash::AshSize GetOffset();
        virtual void* GetLocation();
        template<typename T> 
        inline T* GetLocationAs() 
        { 
            return static_cast<T*>( this->GetLocation() ); 
        }
        virtual ash::AshSize GetSize();
        virtual bool IsValid();

        AshReferenceableHandle& operator=(AshReferenceableHandle Source) = delete;
    private:
        AshReferenceableHandle();
        void Invalidate();

        ASH_CLASS_ADD_INTERNAL_OBJECT(AshReferenceableHandle);
        ASH_CLASS_DECLARE_FRIEND(AshReferenceableBuffer);
    };

}