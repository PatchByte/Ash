#include "Ash/AshBuffer.h"
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
            flags.value = 0;
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
        AshSize bufferSize;
        void* bufferPointer;
    };

    AshBuffer::AshBuffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshBuffer);
        this->ReleaseMemory();
    }

    AshBuffer::AshBuffer(AshPointer Pointer, AshSize Size):
        AshBuffer::AshBuffer()
    {
        this->ImportPointer(Pointer, Size);
    }

    AshBuffer::AshBuffer(AshBuffer& Source)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshBuffer);
        this->ReleaseMemory();
        this->AllocateSize(Source.GetSize());
        memcpy(this->GetPointer(), Source.GetPointer(), Source.GetSize());
    }

    AshBuffer::~AshBuffer()
    {
        // Actual release
        this->ReleaseMemory();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshBuffer);
    }

    bool AshBuffer::AllocateSize(AshSize Size)
    {
        this->ReleaseMemory();

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

    bool AshBuffer::AllocateArray(AshSize ElementSize, AshSize ElementCount)
    {
        this->ReleaseMemory();

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

    bool AshBuffer::ExpandSize(AshSize Size)
    {

        ash::AshSize expandedBufferSize = classInternalAshBuffer->bufferSize + Size;
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

        classInternalAshBuffer->Reset();
        return false;
    }

    bool AshBuffer::ShrinkSize(AshSize Size)
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

    bool AshBuffer::ImportPointer(AshPointer Pointer, AshSize Size)
    {
        this->ReleaseMemory();

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

    bool AshBuffer::CopyPointer(AshPointer Pointer, AshSize Size)
    {
        this->ReleaseMemory();
        if(this->AllocateSize(Size) == false)
        {
            return false;
        }

        memcpy(this->GetPointer(), Pointer, Size);
        return true;
    }

    bool AshBuffer::CopyBytesVector(AshBytesVector Vector)
    {
        this->ReleaseMemory();
        if(this->AllocateSize(Vector.size()) == false)
        {
            return false;
        }

        memcpy(this->GetPointer(), Vector.data(), Vector.size());
        return true;
    }

    bool AshBuffer::CopyAshBufferFromPointer(ash::AshBuffer* Buffer)
    {
        return this->CopyPointer(Buffer->GetPointer(), Buffer->GetSize());
    }

    AshBuffer* AshBuffer::DuplicateAndCopyBuffer()
    {
        AshBuffer* duplicate = new AshBuffer();
        duplicate->CopyPointer(this->GetPointer(), this->GetSize());
        return duplicate;
    }

    bool AshBuffer::SetReleasable(bool Releasable)
    {
        classInternalAshBuffer->flags.bits.isReleasable = Releasable;
        return true;
    }

    bool AshBuffer::ReleaseMemory()
    {
        if(classInternalAshBuffer->bufferPointer != nullptr && classInternalAshBuffer->flags.bits.isReleasable == true)
        {
            // Safety precautions, hehe looking at you @NSA
            memset(classInternalAshBuffer->bufferPointer, 0, classInternalAshBuffer->bufferSize);
            operator delete(classInternalAshBuffer->bufferPointer);
        }

        classInternalAshBuffer->Reset();

        return true;
    }

    bool AshBuffer::IsAllocated()
    {
        return classInternalAshBuffer->flags.bits.isAllocated;
    }

    bool AshBuffer::IsImported()
    {
        return classInternalAshBuffer->flags.bits.isImported;
    }

    bool AshBuffer::IsArray()
    {
        return classInternalAshBuffer->flags.bits.isArray;
    }

    bool AshBuffer::IsReleasable()
    {
        return classInternalAshBuffer->flags.bits.isReleasable;
    }

    void* AshBuffer::GetPointer()
    {
        return classInternalAshBuffer->bufferPointer;
    }

    AshSize AshBuffer::GetSize()
    {
        return classInternalAshBuffer->bufferSize;
    }

    // File Utilities

    AshResult AshBuffer::ReadFromFile(std::filesystem::path Path)
    {
        ReleaseMemory();

        if(std::filesystem::exists(Path) == false)
        {
            return AshResult(false, "File does not exist.");
        }

        std::ifstream istream = std::ifstream();
        ash::AshSize istreamSize = 0;

        istream.open(Path, std::ios::binary);

        if(istream.bad())
        {
            istream.close();
            return AshResult(false, "Stream is bad.");
        }

        istream.seekg(0, std::ios::end);
        istreamSize = istream.tellg();
        istream.seekg(0);

        if(istreamSize <= 0)
        {
            return AshResult(false, "File size is zero or well less.");
        }
        
        this->AllocateSize(istreamSize);
        istream.read(reinterpret_cast<char*>(this->GetPointer()), istreamSize);
        istream.close();

        return AshResult(true);
    }

    AshResult AshBuffer::WriteToFile(std::filesystem::path Path)
    {
        if(this->GetPointer() == nullptr)
        {
            return AshResult(false, "Buffer is nullptr.");
        }

        std::ofstream ostream = std::ofstream();

        ostream.open(Path, std::ios::binary | std::ios::trunc);

        if(ostream.bad())
        {
            return AshResult(false, "Stream is bad.");
        }

        ostream.write(reinterpret_cast<char*>(this->GetPointer()), this->GetSize());
        ostream.close();

        return AshResult(true);
    }

    // Memory operations

    bool AshBuffer::ClearBytesWithValue(unsigned char Value)
    {
        if(classInternalAshBuffer->flags.bits.isAllocated == false)
        {
            return false;
        }

        memset(classInternalAshBuffer->bufferPointer, 0, classInternalAshBuffer->bufferSize);
        return true;
    }

    AshBuffer& AshBuffer::operator=(AshBuffer Source)
    {
        this->ReleaseMemory();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshBuffer);
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshBuffer);
        this->AllocateSize(Source.GetSize());
        memcpy(this->GetPointer(), Source.GetPointer(), Source.GetSize());
        return *this;
    }

}