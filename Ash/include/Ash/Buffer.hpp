#ifndef ASH_BUFFER_H
#define ASH_BUFFER_H

#include "Class.hpp"
#include "Result.hpp"

#include <filesystem>
#include <vector>

namespace ash
{
    using Pointer = void*;
    using Size = unsigned long long;
    using BytesVector = std::vector<unsigned char>;

    class Buffer
    {
    public:
        Buffer();
        // Import Constructor
        Buffer(Pointer Pointer, Size Size);
        Buffer(Buffer& Source);
        virtual ~Buffer();

        virtual bool AllocateBySize(Size Size);
        virtual bool AllocateArray(Size ElementSize, Size ElementCount);

        template <typename T>
        inline bool AllocateByType()
        {
            return this->AllocateBySize(sizeof(T));
        }

        virtual bool ExpandSize(Size Size);
        virtual bool ShrinkSize(Size Size);

        // This just sets the pointer and does not deallocate

        virtual bool ImportPointer(Pointer Pointer, Size Size);

        // This will actually allocate memory that will be later on released.

        virtual bool CopyPointer(Pointer Pointer, Size Size);
        virtual bool CopyBytesVector(BytesVector Vector);
        virtual bool CopyBufferFromPointer(ash::Buffer* Buffer);

        virtual bool CopyBuffer(ash::Buffer& Buffer)
        {
            return this->CopyBufferFromPointer(&Buffer);
        }

        // Allocates a new AshBuffer and copies the original AshBuffer
        virtual Buffer* DuplicateAndCopy();

        // ReleaseMemory is also like a sort of "Reset" function
        virtual bool Release();

        virtual bool SetReleasable(bool Releasable);
        virtual bool MarkAsConsumed();
        virtual bool IsAllocated();
        virtual bool IsImported();
        virtual bool IsArray();
        virtual bool IsReleasable();

        template <typename T>
        T* GetBuffer()
        {
            return static_cast<T*>(GetPointer());
        }

        virtual unsigned char* GetBytes()
        {
            return static_cast<unsigned char*>(GetPointer());
        }

        inline virtual BytesVector GetBytesAsVector()
        {
            BytesVector vector = {};
            vector.insert(vector.end(), GetBytes(), GetBytes() + GetSize());
            return vector;
        }

        template <typename T>
        inline T* GetBufferAtOffset(ash::Size Offset)
        {
            if (GetSize() <= Offset)
            {
                // This should not be reached.
                return nullptr;
            }

            return reinterpret_cast<T*>(GetBytes() + Offset);
        }

        virtual Pointer GetPointer();
        virtual Size GetSize();

        // File Utils
        virtual Result ReadFromFile(std::filesystem::path Path);
        virtual Result WriteToFile(std::filesystem::path Path);

        // Memory operations

        virtual std::string GetAsString();
        virtual bool SetAsString(std::string Text);

        // memset's the buffer with the specified value.
        virtual bool ClearBytesWithValue(unsigned char Value);

        // Calls ClearBytesWithValue with the value of 0
        virtual bool ClearBytes()
        {
            return ClearBytesWithValue(0);
        }

        // Assignment initialization
        Buffer& operator=(Buffer Source);

    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshBuffer);
    };

} // namespace ash

#endif // !ASH_BUFFER_H
