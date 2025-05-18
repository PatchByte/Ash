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
            bufferMode = StreamMode::INVALID;
            bufferCursor = 0;
            flags.value = 0;
        }

        void SetErrorHasOccurred()
        {
            flags.bits.hasErrorOccurred = true;
        }

        Buffer* buffer;
        StreamMode bufferMode;
        Size bufferCursor;

        union
        {
            unsigned char value;
            struct {
                bool hasErrorOccurred;
                bool isSingleUseBuffer;
            } bits;
        } flags;
    };

    StreamStaticBuffer::StreamStaticBuffer(ash::Buffer* StaticBuffer, StreamMode StaticBufferMode)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshStreamStaticBuffer);
        classInternalAshStreamStaticBuffer->Reset();
        classInternalAshStreamStaticBuffer->buffer = StaticBuffer;
        classInternalAshStreamStaticBuffer->bufferMode = StaticBufferMode;
        classInternalAshStreamStaticBuffer->bufferCursor = 0;
    }

    StreamStaticBuffer::~StreamStaticBuffer()
    {
        if(classInternalAshStreamStaticBuffer->flags.bits.isSingleUseBuffer)
        {
            delete classInternalAshStreamStaticBuffer->buffer;
            classInternalAshStreamStaticBuffer->buffer = nullptr;
        }
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshStreamStaticBuffer);
    }

    StreamStaticBuffer* StreamStaticBuffer::DeallocateBufferAfterUse()
    {
        classInternalAshStreamStaticBuffer->flags.bits.isSingleUseBuffer = true;
        return this;
    }

    Result StreamStaticBuffer::ReadRawIntoPointer(void* Buffer, Size BufferSize)
    {
        if(IsReadOnly() == false)
        {
            return Result(false, "Stream is write only.");
        }

        if(IsEndOfStream())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return Result(false, "Stream is eof.");
        }

        if((classInternalAshStreamStaticBuffer->bufferCursor + BufferSize) > classInternalAshStreamStaticBuffer->buffer->GetSize())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return Result(false, "Size is overlapping the eof.");
        }

        for(Size i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(Buffer)[i] = static_cast<unsigned char*>(classInternalAshStreamStaticBuffer->buffer->GetBytes())[classInternalAshStreamStaticBuffer->bufferCursor];
            ++classInternalAshStreamStaticBuffer->bufferCursor;
        }

        return Result(true);
    }

    Result StreamStaticBuffer::WriteRawFromPointer(void* Buffer, Size BufferSize)
    {
        if(IsWriteOnly() == false)
        {
            return Result(false, "Stream is read only.");
        }

        if(IsEndOfStream())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return Result(false, "Stream is eof.");
        }

        if((classInternalAshStreamStaticBuffer->bufferCursor + BufferSize) > classInternalAshStreamStaticBuffer->buffer->GetSize())
        {
            classInternalAshStreamStaticBuffer->SetErrorHasOccurred();
            return Result(false, "Size is overlapping the eof.");
        }

        for(Size i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(classInternalAshStreamStaticBuffer->buffer->GetBytes())[classInternalAshStreamStaticBuffer->bufferCursor] = static_cast<unsigned char*>(Buffer)[i];
            ++classInternalAshStreamStaticBuffer->bufferCursor;
        }

        return Result(true);
    }

    bool StreamStaticBuffer::IsEndOfStream()
    {
        return classInternalAshStreamStaticBuffer->bufferCursor >= classInternalAshStreamStaticBuffer->buffer->GetSize();
    }

    bool StreamStaticBuffer::HasErrorOccurred()
    {
        return classInternalAshStreamStaticBuffer->flags.bits.hasErrorOccurred;
    }

    StreamMode StreamStaticBuffer::GetStreamMode()
    {
        return classInternalAshStreamStaticBuffer->bufferMode;
    }

    ash::Size StreamStaticBuffer::GetCursorPosition()
    {
        return classInternalAshStreamStaticBuffer->bufferCursor;
    }

    ash::Result StreamStaticBuffer::SetCursorPosition(ash::Size CursorPosition)
    {
        if( CursorPosition > ((classInternalAshStreamStaticBuffer->buffer->GetSize()) - 1) )
        {
            return ash::Result(false, "The cursor position is larger than the buffer size.");
        }

        classInternalAshStreamStaticBuffer->bufferCursor = CursorPosition;

        return true;
    }

    // AshStreamExpandableExportBuffer

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshStreamExpandableExportBuffer)
    {
    public:
        ClassAshStreamExpandableExportBufferInternalObject():
            expandableBuffer(nullptr),
            expandableBufferCursor(0),
            hasErrorOccurred(false)
        {}

        void Reset()
        {
            if(expandableBuffer != nullptr)
            {
                delete expandableBuffer;
                expandableBuffer = nullptr;
            }

            expandableBufferCursor = 0;
            hasErrorOccurred = false;
        }

        void SetErrorHasOccurred()
        {
            hasErrorOccurred = true;
        }

        ash::Buffer* expandableBuffer;
        ash::Size expandableBufferCursor;
        bool hasErrorOccurred;
    };

    StreamExpandableExportBuffer::StreamExpandableExportBuffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshStreamExpandableExportBuffer);
        classInternalAshStreamExpandableExportBuffer->expandableBuffer = new ash::Buffer();
    }

    StreamExpandableExportBuffer::~StreamExpandableExportBuffer()
    {
        classInternalAshStreamExpandableExportBuffer->Reset();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshStreamExpandableExportBuffer);
    }

    ash::Buffer* StreamExpandableExportBuffer::MakeCopyOfBuffer()
    {
        return classInternalAshStreamExpandableExportBuffer->expandableBuffer->DuplicateAndCopy();
    }

    Result StreamExpandableExportBuffer::ReadRawIntoPointer(void* Buffer, Size BufferSize)
    {
        return ash::Result(false, "ReadRawIntoPointer is not implemented because AshStreamExpandableExportBuffer is write only . ;-)");
    }

    Result StreamExpandableExportBuffer::WriteRawFromPointer(void* Buffer, Size BufferSize)
    {
        classInternalAshStreamExpandableExportBuffer->expandableBuffer->ExpandSize(BufferSize);

        for(Size i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(classInternalAshStreamExpandableExportBuffer->expandableBuffer->GetBytes())[classInternalAshStreamExpandableExportBuffer->expandableBufferCursor] = static_cast<unsigned char*>(Buffer)[i];
            ++classInternalAshStreamExpandableExportBuffer->expandableBufferCursor;
        }

        return ash::Result(true);
    }

    bool StreamExpandableExportBuffer::HasErrorOccurred()
    {
        return classInternalAshStreamExpandableExportBuffer->hasErrorOccurred;
    }

    ash::Size StreamExpandableExportBuffer::GetCursorPosition()
    {
        return classInternalAshStreamExpandableExportBuffer->expandableBufferCursor;
    }
}
