#include "Ash/AshStream.h"

namespace ash
{

    // AshStreamStaticBuffer

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshStreamStaticBuffer)
    {
    public:
        void Reset()
        {
            buffer = nullptr;
            bufferMode = AshStreamMode::INVALID;
            bufferCursor = 0;
            flags.value = 0;
        }

        void SetErrorHasOccurred()
        {
            flags.bits.hasErrorOccurred = true;
        }

        AshBuffer* buffer;
        AshStreamMode bufferMode;
        AshSize bufferCursor;

        union
        {
            unsigned char value;
            struct {
                bool hasErrorOccurred;
                bool isSingleUseBuffer;
            } bits;
        } flags;
    };

    AshStreamStaticBuffer::AshStreamStaticBuffer(ash::AshBuffer* StaticBuffer, AshStreamMode StaticBufferMode)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshStreamStaticBuffer);
        classInternalAshStreamStaticBuffer->Reset();
        classInternalAshStreamStaticBuffer->buffer = StaticBuffer;
        classInternalAshStreamStaticBuffer->bufferMode = StaticBufferMode;
        classInternalAshStreamStaticBuffer->bufferCursor = 0;
    }

    AshStreamStaticBuffer::~AshStreamStaticBuffer()
    {
        if(classInternalAshStreamStaticBuffer->flags.bits.isSingleUseBuffer)
        {
            delete classInternalAshStreamStaticBuffer->buffer;
            classInternalAshStreamStaticBuffer->buffer = nullptr;
        }
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshStreamStaticBuffer);
    }

    AshStreamStaticBuffer* AshStreamStaticBuffer::DeallocateBufferAfterUse()
    {
        classInternalAshStreamStaticBuffer->flags.bits.isSingleUseBuffer = true;
        return this;
    }

    AshResult AshStreamStaticBuffer::ReadRawIntoPointer(void* Buffer, AshSize BufferSize)
    {
        if(IsReadOnly() == false)
        {
            return AshResult(false, "Stream is write only.");
        }

        if(IsEndOfStream())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return AshResult(false, "Stream is eof.");
        }

        if((classInternalAshStreamStaticBuffer->bufferCursor + BufferSize) > classInternalAshStreamStaticBuffer->buffer->GetSize())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return AshResult(false, "Size is overlapping the eof.");
        }

        for(AshSize i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(Buffer)[i] = static_cast<unsigned char*>(classInternalAshStreamStaticBuffer->buffer->GetBytes())[classInternalAshStreamStaticBuffer->bufferCursor];
            ++classInternalAshStreamStaticBuffer->bufferCursor;
        }

        return AshResult(true);
    }

    AshResult AshStreamStaticBuffer::WriteRawFromPointer(void* Buffer, AshSize BufferSize)
    {
        if(IsWriteOnly() == false)
        {
            return AshResult(false, "Stream is read only.");
        }

        if(IsEndOfStream())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return AshResult(false, "Stream is eof.");
        }

        if((classInternalAshStreamStaticBuffer->bufferCursor + BufferSize) > classInternalAshStreamStaticBuffer->buffer->GetSize())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return AshResult(false, "Size is overlapping the eof.");
        }

        for(AshSize i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(classInternalAshStreamStaticBuffer->buffer->GetBytes())[classInternalAshStreamStaticBuffer->bufferCursor] = static_cast<unsigned char*>(Buffer)[i];
            ++classInternalAshStreamStaticBuffer->bufferCursor;
        }

        return AshResult(true);
    }

    bool AshStreamStaticBuffer::IsEndOfStream()
    {
        return classInternalAshStreamStaticBuffer->bufferCursor >= classInternalAshStreamStaticBuffer->buffer->GetSize();
    }

    bool AshStreamStaticBuffer::HasErrorOccurred()
    {
        return classInternalAshStreamStaticBuffer->flags.bits.hasErrorOccurred;
    }

    AshStreamMode AshStreamStaticBuffer::GetStreamMode()
    {
        return classInternalAshStreamStaticBuffer->bufferMode;
    }

    // AshStreamExpandableExportBuffer

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshStreamExpandableExportBuffer)
    {
    public:
        ash::AshBuffer* expandableBuffer;
    };

    AshStreamExpandableExportBuffer::AshStreamExpandableExportBuffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshStreamExpandableExportBuffer);
        classInternalAshStreamExpandableExportBuffer->expandableBuffer = new ash::AshBuffer();
    }

    AshStreamExpandableExportBuffer::~AshStreamExpandableExportBuffer()
    {
        if(classInternalAshStreamExpandableExportBuffer->expandableBuffer)
        {
            delete classInternalAshStreamExpandableExportBuffer->expandableBuffer;
            classInternalAshStreamExpandableExportBuffer->expandableBuffer = nullptr;
        }

        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshStreamExpandableExportBuffer);
    }

    ash::AshBuffer* AshStreamExpandableExportBuffer::GetBuffer()
    {
        return classInternalAshStreamExpandableExportBuffer->expandableBuffer;
    }
    
    AshResult AshStreamExpandableExportBuffer::ReadRawIntoPointer(void* Buffer, AshSize BufferSize)
    {
        return ash::AshResult(false, "ReadRawIntoPointer is not allowed here.");
    }

    AshResult AshStreamExpandableExportBuffer::WriteRawFromPointer(void* Buffer, AshSize BufferSize)
    {
        return ash::AshResult(false, "WriteRawFromPointer is not implemented yet because I am riding my bike in the woods.");
    }

}