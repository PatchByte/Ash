#pragma once
#include "AshClass.h"

namespace ash
{
    using AshPointer = void*;
    using AshSize = size_t;

    class AshBuffer
    {
    public:
        AshBuffer();
        virtual ~AshBuffer();

        virtual bool AllocateSize(AshSize Size);
        virtual bool AllocateArray(AshSize ElementSize, AshSize ElementCount);
        template<typename T>
        inline bool AllocateType() { return this->AllocateSize(sizeof(T)); }

        virtual bool ImportPointer(AshPointer Pointer, AshSize Size);

        // ReleaseMemory is also like a sort of "Reset" function
        virtual bool ReleaseMemory();

        virtual bool SetReleasable(bool Releasable);

        virtual bool IsAllocated();
        virtual bool IsImported();
        virtual bool IsArray();
        virtual bool IsReleasable();

        template<typename T> T* GetBuffer() { return static_cast< T* >( GetPointer() ); }
        virtual unsigned char* GetBytes() { return static_cast<unsigned char*>( GetPointer() ); }
        
        virtual AshPointer GetPointer();
        AshSize GetSize();
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshBuffer);
    };

}