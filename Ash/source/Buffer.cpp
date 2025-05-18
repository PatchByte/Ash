#include "Ash/Buffer.h"
#include <cstring>
#include <fstream>
#include <iostream>

namespace ash
{

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshBuffer)
    {
    public:
        ClassAshBufferInternalObject()
        {
            this->Reset();
        }

        // You may ask "Why is this not in hte AshBuffer.h?", simple answer, it's dangerous, you know memory leaky stuff...
        void Reset()
        {
            m_Flags.m_Value = 0;
            m_BufferSize = 0;
            m_BufferPointer = nullptr;
        }

        union
        {
            unsigned char m_Value;

            struct
            {
                bool m_IsAllocated : 1;
                bool m_IsImported : 1;
                bool m_IsArray : 1;
                bool m_IsReleasable : 1;
            } m_Bits;
        } m_Flags;

        Size m_BufferSize;
        void* m_BufferPointer;
    };

    Buffer::Buffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshBuffer);
        this->Release();
    }

    Buffer::Buffer(Pointer Pointer, Size Size) : Buffer::Buffer()
    {
        this->ImportPointer(Pointer, Size);
    }

    Buffer::Buffer(Buffer& Source)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshBuffer);
        this->Release();
        this->AllocateSize(Source.GetSize());
        memcpy(this->GetPointer(), Source.GetPointer(), Source.GetSize());
    }

    Buffer::~Buffer()
    {
        // Actual release
        this->Release();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshBuffer);
    }

    bool Buffer::AllocateSize(Size Size)
    {
        this->Release();

        classInternalAshBuffer->m_Flags.m_Bits.m_IsAllocated = true;
        classInternalAshBuffer->m_Flags.m_Bits.m_IsReleasable = true;

        classInternalAshBuffer->m_BufferSize = Size;
        classInternalAshBuffer->m_BufferPointer = operator new(Size);

        memset(classInternalAshBuffer->m_BufferPointer, 0, classInternalAshBuffer->m_BufferSize);

        if (classInternalAshBuffer->m_BufferPointer != nullptr)
        {
            return true;
        }

        classInternalAshBuffer->Reset();
        return false;
    }

    bool Buffer::AllocateArray(Size ElementSize, Size ElementCount)
    {
        this->Release();

        classInternalAshBuffer->m_Flags.m_Bits.m_IsAllocated = true;
        classInternalAshBuffer->m_Flags.m_Bits.m_IsArray = true;
        classInternalAshBuffer->m_Flags.m_Bits.m_IsReleasable = true;

        classInternalAshBuffer->m_BufferSize = ElementSize * ElementCount;
        classInternalAshBuffer->m_BufferPointer = operator new(ElementSize * ElementCount);

        memset(classInternalAshBuffer->m_BufferPointer, 0, classInternalAshBuffer->m_BufferSize);

        if (classInternalAshBuffer->m_BufferPointer != nullptr)
        {
            return true;
        }

        classInternalAshBuffer->Reset();
        return false;
    }

    bool Buffer::ExpandSize(Size Size)
    {

        if (this->IsAllocated())
        {
            ash::Size expandedBufferSize = classInternalAshBuffer->m_BufferSize + Size;
            void* expandedBufferPointer = operator new(expandedBufferSize);

            memset(expandedBufferPointer, 0, expandedBufferSize);
            memcpy(expandedBufferPointer, classInternalAshBuffer->m_BufferPointer, classInternalAshBuffer->m_BufferSize);

            operator delete(classInternalAshBuffer->m_BufferPointer);

            classInternalAshBuffer->m_BufferPointer = expandedBufferPointer;
            classInternalAshBuffer->m_BufferSize = expandedBufferSize;

            if (classInternalAshBuffer->m_BufferPointer != nullptr)
            {
                return true;
            }
        }
        else
        {
            return this->AllocateSize(Size);
        }

        classInternalAshBuffer->Reset();
        return false;
    }

    bool Buffer::ShrinkSize(Size Size)
    {
        signed long long shrunkBufferSize = ((signed long long)classInternalAshBuffer->m_BufferSize) - ((signed long long)Size);

        if (shrunkBufferSize <= 0)
        {
            classInternalAshBuffer->Reset();
            return false;
        }

        void* shrunkBufferPointer = operator new(shrunkBufferSize);

        memset(shrunkBufferPointer, 0, shrunkBufferSize);
        memcpy(shrunkBufferPointer, classInternalAshBuffer->m_BufferPointer, shrunkBufferSize);

        operator delete(classInternalAshBuffer->m_BufferPointer);

        classInternalAshBuffer->m_BufferPointer = shrunkBufferPointer;
        classInternalAshBuffer->m_BufferSize = shrunkBufferSize;

        return true;
    }

    bool Buffer::ImportPointer(Pointer Pointer, Size Size)
    {
        this->Release();

        classInternalAshBuffer->m_Flags.m_Bits.m_IsAllocated = true;
        classInternalAshBuffer->m_Flags.m_Bits.m_IsReleasable = false;

        classInternalAshBuffer->m_BufferSize = Size;
        classInternalAshBuffer->m_BufferPointer = Pointer;

        if (classInternalAshBuffer->m_BufferPointer != nullptr)
        {
            return true;
        }

        classInternalAshBuffer->Reset();
        return false;
    }

    bool Buffer::CopyPointer(Pointer Pointer, Size Size)
    {
        this->Release();
        if (this->AllocateSize(Size) == false)
        {
            return false;
        }

        memcpy(this->GetPointer(), Pointer, Size);
        return true;
    }

    bool Buffer::CopyBytesVector(BytesVector Vector)
    {
        this->Release();
        if (this->AllocateSize(Vector.size()) == false)
        {
            return false;
        }

        memcpy(this->GetPointer(), Vector.data(), Vector.size());
        return true;
    }

    bool Buffer::CopyBufferFromPointer(ash::Buffer* Buffer)
    {
        return this->CopyPointer(Buffer->GetPointer(), Buffer->GetSize());
    }

    Buffer* Buffer::DuplicateAndCopy()
    {
        Buffer* duplicate = new Buffer();
        duplicate->CopyPointer(this->GetPointer(), this->GetSize());
        return duplicate;
    }

    bool Buffer::SetReleasable(bool Releasable)
    {
        classInternalAshBuffer->m_Flags.m_Bits.m_IsReleasable = Releasable;
        return true;
    }

    bool Buffer::MarkAsConsumed()
    {
        classInternalAshBuffer->m_Flags.m_Bits.m_IsReleasable = false;
        return true;
    }

    bool Buffer::Release()
    {
        if (classInternalAshBuffer->m_BufferPointer != nullptr && classInternalAshBuffer->m_Flags.m_Bits.m_IsReleasable == true)
        {
            memset(classInternalAshBuffer->m_BufferPointer, 0, classInternalAshBuffer->m_BufferSize);
            operator delete(classInternalAshBuffer->m_BufferPointer);
            classInternalAshBuffer->m_BufferPointer = nullptr;
        }

        classInternalAshBuffer->Reset();

        return true;
    }

    bool Buffer::IsAllocated()
    {
        return classInternalAshBuffer->m_Flags.m_Bits.m_IsAllocated;
    }

    bool Buffer::IsImported()
    {
        return classInternalAshBuffer->m_Flags.m_Bits.m_IsImported;
    }

    bool Buffer::IsArray()
    {
        return classInternalAshBuffer->m_Flags.m_Bits.m_IsArray;
    }

    bool Buffer::IsReleasable()
    {
        return classInternalAshBuffer->m_Flags.m_Bits.m_IsReleasable;
    }

    void* Buffer::GetPointer()
    {
        return classInternalAshBuffer->m_BufferPointer;
    }

    Size Buffer::GetSize()
    {
        return classInternalAshBuffer->m_BufferSize;
    }

    // File Utilities

    Result Buffer::ReadFromFile(std::filesystem::path Path)
    {
        Release();

        if (std::filesystem::exists(Path) == false)
        {
            return Result(false, "File does not exist.");
        }

        std::ifstream istream = std::ifstream();
        ash::Size istreamSize = 0;

        istream.open(Path, std::ios::binary);

        if (istream.bad())
        {
            istream.close();
            return Result(false, "Stream is bad.");
        }

        istream.seekg(0, std::ios::end);
        istreamSize = istream.tellg();
        istream.seekg(0);

        if (istreamSize <= 0)
        {
            return Result(false, "File size is zero or well less.");
        }

        this->AllocateSize(istreamSize);
        istream.read(reinterpret_cast<char*>(this->GetPointer()), istreamSize);
        istream.close();

        return Result(true);
    }

    Result Buffer::WriteToFile(std::filesystem::path Path)
    {
        if (this->GetPointer() == nullptr)
        {
            return Result(false, "Buffer is nullptr.");
        }

        std::ofstream ostream = std::ofstream();

        ostream.open(Path, std::ios::binary | std::ios::trunc);

        if (ostream.bad())
        {
            return Result(false, "Stream is bad.");
        }

        ostream.write(reinterpret_cast<char*>(this->GetPointer()), this->GetSize());
        ostream.close();

        return Result(true);
    }

    // Memory operations

    std::string Buffer::GetAsString()
    {
        return std::string(GetBuffer<char>(), GetSize());
    }

    bool Buffer::SetAsString(std::string Text)
    {
        return ImportPointer(Text.data(), Text.size());
    }

    bool Buffer::ClearBytesWithValue(unsigned char Value)
    {
        if (classInternalAshBuffer->m_Flags.m_Bits.m_IsAllocated == false)
        {
            return false;
        }

        memset(classInternalAshBuffer->m_BufferPointer, 0, classInternalAshBuffer->m_BufferSize);
        return true;
    }

    Buffer& Buffer::operator=(Buffer Source)
    {
        this->Release();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshBuffer);
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshBuffer);
        this->AllocateSize(Source.GetSize());
        memcpy(this->GetPointer(), Source.GetPointer(), Source.GetSize());
        return *this;
    }

} // namespace ash
