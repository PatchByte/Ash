#pragma once
#include "AshClass.h"
#include <vector>

namespace ash
{
    using AshPointer = void*;
    using AshSize = size_t;
    using AshBytesVector = std::vector<unsigned char>;

    class AshBuffer
    {
    public:
        AshBuffer();
        // Import Constructor
        AshBuffer(AshPointer Pointer, AshSize Size);
        virtual ~AshBuffer();

        virtual bool AllocateSize(AshSize Size);
        virtual bool AllocateArray(AshSize ElementSize, AshSize ElementCount);
        template<typename T>
        inline bool AllocateType() { return this->AllocateSize(sizeof(T)); }

        virtual bool ImportPointer(AshPointer Pointer, AshSize Size);

        // This will actually allocate memory that will be later on released.
        virtual bool ImportBytesVector(AshBytesVector Vector);

        // ReleaseMemory is also like a sort of "Reset" function
        virtual bool ReleaseMemory();

        virtual bool SetReleasable(bool Releasable);

        virtual bool IsAllocated();
        virtual bool IsImported();
        virtual bool IsArray();
        virtual bool IsReleasable();

        template<typename T> T* GetBuffer() { return static_cast< T* >( GetPointer() ); }
        virtual unsigned char* GetBytes() { return static_cast<unsigned char*>( GetPointer() ); }
        inline virtual AshBytesVector GetBytesAsVector() 
        {
            AshBytesVector vector = {};
            vector.insert(vector.end(), GetBytes(), GetBytes() + GetSize());
            return vector;
        }
        
        virtual AshPointer GetPointer();
        AshSize GetSize();
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshBuffer);
    };

}