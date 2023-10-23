#pragma once
#include "AshClass.h"
#include "AshResult.h"

#include <iostream>
#include <filesystem>
#include <vector>

namespace ash
{
    using AshPointer = void*;
    using AshSize = unsigned long long;
    using AshBytesVector = std::vector<unsigned char>;

    enum class AshBufferStreamMode
    {
        INVALID,
        READ,
        WRITE
    };

    class AshBuffer
    {
    public:
        AshBuffer();
        // Import Constructor
        AshBuffer(AshPointer Pointer, AshSize Size);
        AshBuffer(AshBuffer& Source);
        virtual ~AshBuffer();

        virtual bool AllocateSize(AshSize Size);
        virtual bool AllocateArray(AshSize ElementSize, AshSize ElementCount);
        template<typename T>
        inline bool AllocateType() { return this->AllocateSize(sizeof(T)); }

        virtual bool ExpandSize(AshSize Size);
        virtual bool ShrinkSize(AshSize Size);

        // This just sets the pointer and does not deallocate

        virtual bool ImportPointer(AshPointer Pointer, AshSize Size);

        // This will actually allocate memory that will be later on released.
        
        virtual bool CopyPointer(AshPointer Pointer, AshSize Size);
        virtual bool CopyBytesVector(AshBytesVector Vector);
        virtual bool CopyAshBufferFromPointer(ash::AshBuffer* Buffer);
        virtual bool CopyAshBuffer(ash::AshBuffer& Buffer) { return this->CopyAshBufferFromPointer(&Buffer); }

        // Allocates a new AshBuffer and copies the original AshBuffer
        virtual AshBuffer* DuplicateAndCopyBuffer();

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

        template<typename T> 
        inline T* GetBufferAtOffset(ash::AshSize Offset) 
        { 
            if(GetSize() <= Offset) 
            { 
                // This should not be reached.
                return nullptr;
            } 
        
            return static_cast<T*>(GetBytes() + Offset); 
        }
        
        virtual AshPointer GetPointer();
        virtual AshSize GetSize();

        // File Utils
        virtual AshResult ReadFromFile(std::filesystem::path Path);
        virtual AshResult WriteToFile(std::filesystem::path Path);

        // Memory operations

        // memset's the buffer with the specified value.
        virtual bool ClearBytesWithValue(unsigned char Value);
        // Calls ClearBytesWithValue with the value of 0
        virtual bool ClearBytes() { return ClearBytesWithValue(0); }

        // Assignment initialization
        AshBuffer& operator=(AshBuffer Source);
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshBuffer);
    };

}