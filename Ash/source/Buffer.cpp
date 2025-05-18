#include "Ash/Buffer.h"
#include <iostream>
#include <cstring>
#include <fstream>

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
            flags.value = 0;
            bufferSize = 0;
            bufferPointer = nullptr;
        }

        union {
            unsigned char value;
            struct {
                bool isAllocated : 1;
                bool isImported : 1;
                bool isArray : 1;
                bool isReleasable : 1;
            } bits;
        } flags;
        Size bufferSize;
        void* bufferPointer;
    };

    Buffer::Buffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshBuffer);
        this->Release();
    }

    Buffer::Buffer(Pointer Pointer, Size Size):
        Buffer::Buffer()
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

        classInternalAshBuffer->flags.bits.isAllocated = true;
        classInternalAshBuffer->flags.bits.isReleasable = true;

        classInternalAshBuffer->bufferSize = Size;
        classInternalAshBuffer->bufferPointer = operator new(Size);

        memset(classInternalAshBuffer->bufferPointer, 0, classInternalAshBuffer->bufferSize);

        if(classInternalAshBuffer->bufferPointer != nullptr)
        {
            return true;
        }

        classInternalAshBuffer->Reset();
        return false;
    }

    bool Buffer::AllocateArray(Size ElementSize, Size ElementCount)
    {
        this->Release();

        classInternalAshBuffer->flags.bits.isAllocated = true;
        classInternalAshBuffer->flags.bits.isArray = true;
        classInternalAshBuffer->flags.bits.isReleasable = true;

        classInternalAshBuffer->bufferSize = ElementSize * ElementCount;
        classInternalAshBuffer->bufferPointer = operator new(ElementSize * ElementCount);

        memset(classInternalAshBuffer->bufferPointer, 0, classInternalAshBuffer->bufferSize);

        if(classInternalAshBuffer->bufferPointer != nullptr)
        {
            return true;
        }

        classInternalAshBuffer->Reset();
        return false;
    }

    bool Buffer::ExpandSize(Size Size)
    {

        if(this->IsAllocated())
        {
            ash::Size expandedBufferSize = classInternalAshBuffer->bufferSize + Size;
            void* expandedBufferPointer = operator new(expandedBufferSize);

            memset(expandedBufferPointer, 0, expandedBufferSize);
            memcpy(expandedBufferPointer, classInternalAshBuffer->bufferPointer, classInternalAshBuffer->bufferSize);

            operator delete(classInternalAshBuffer->bufferPointer);

            classInternalAshBuffer->bufferPointer = expandedBufferPointer;
            classInternalAshBuffer->bufferSize = expandedBufferSize;

            if(classInternalAshBuffer->bufferPointer != nullptr)
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
        signed long long shrunkBufferSize = ((signed long long)classInternalAshBuffer->bufferSize) - ((signed long long)Size);

        if(shrunkBufferSize <= 0)
        {
            classInternalAshBuffer->Reset();
            return false;
        }

        void* shrunkBufferPointer = operator new(shrunkBufferSize);

        memset(shrunkBufferPointer, 0, shrunkBufferSize);
        memcpy(shrunkBufferPointer, classInternalAshBuffer->bufferPointer, shrunkBufferSize);

        operator delete(classInternalAshBuffer->bufferPointer);

        classInternalAshBuffer->bufferPointer = shrunkBufferPointer;
        classInternalAshBuffer->bufferSize = shrunkBufferSize;

        return true;
    }

    bool Buffer::ImportPointer(Pointer Pointer, Size Size)
    {
        this->Release();

        classInternalAshBuffer->flags.bits.isAllocated = true;
        classInternalAshBuffer->flags.bits.isReleasable = false;

        classInternalAshBuffer->bufferSize = Size;
        classInternalAshBuffer->bufferPointer = Pointer;

        if(classInternalAshBuffer->bufferPointer != nullptr)
        {
            return true;
        }

        classInternalAshBuffer->Reset();
        return false;
    }

    bool Buffer::CopyPointer(Pointer Pointer, Size Size)
    {
        this->Release();
        if(this->AllocateSize(Size) == false)
        {
            return false;
        }

        memcpy(this->GetPointer(), Pointer, Size);
        return true;
    }

    bool Buffer::CopyBytesVector(BytesVector Vector)
    {
        this->Release();
        if(this->AllocateSize(Vector.size()) == false)
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
        classInternalAshBuffer->flags.bits.isReleasable = Releasable;
        return true;
    }


    bool Buffer::MarkAsConsumed()
    {
        classInternalAshBuffer->flags.bits.isReleasable = false;
        return true;
    }

    bool Buffer::Release()
    {
        if(classInternalAshBuffer->bufferPointer != nullptr && classInternalAshBuffer->flags.bits.isReleasable == true)
        {
            memset(classInternalAshBuffer->bufferPointer, 0, classInternalAshBuffer->bufferSize);
            operator delete(classInternalAshBuffer->bufferPointer);
            classInternalAshBuffer->bufferPointer = nullptr;
        }

        classInternalAshBuffer->Reset();

        return true;
    }

    bool Buffer::IsAllocated()
    {
        return classInternalAshBuffer->flags.bits.isAllocated;
    }

    bool Buffer::IsImported()
    {
        return classInternalAshBuffer->flags.bits.isImported;
    }

    bool Buffer::IsArray()
    {
        return classInternalAshBuffer->flags.bits.isArray;
    }

    bool Buffer::IsReleasable()
    {
        return classInternalAshBuffer->flags.bits.isReleasable;
    }

    void* Buffer::GetPointer()
    {
        return classInternalAshBuffer->bufferPointer;
    }

    Size Buffer::GetSize()
    {
        return classInternalAshBuffer->bufferSize;
    }

    // File Utilities

    Result Buffer::ReadFromFile(std::filesystem::path Path)
    {
        Release();

        if(std::filesystem::exists(Path) == false)
        {
            return Result(false, "File does not exist.");
        }

        std::ifstream istream = std::ifstream();
        ash::Size istreamSize = 0;

        istream.open(Path, std::ios::binary);

        if(istream.bad())
        {
            istream.close();
            return Result(false, "Stream is bad.");
        }

        istream.seekg(0, std::ios::end);
        istreamSize = istream.tellg();
        istream.seekg(0);

        if(istreamSize <= 0)
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
        if(this->GetPointer() == nullptr)
        {
            return Result(false, "Buffer is nullptr.");
        }

        std::ofstream ostream = std::ofstream();

        ostream.open(Path, std::ios::binary | std::ios::trunc);

        if(ostream.bad())
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
        if(classInternalAshBuffer->flags.bits.isAllocated == false)
        {
            return false;
        }

        memset(classInternalAshBuffer->bufferPointer, 0, classInternalAshBuffer->bufferSize);
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

}
