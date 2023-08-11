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

    ash::AshSize AshStreamStaticBuffer::GetCursorPosition()
    {
        return classInternalAshStreamStaticBuffer->bufferCursor;
    }

    ash::AshResult AshStreamStaticBuffer::SetCursorPosition(ash::AshSize CursorPosition)
    {
        if( CursorPosition > ((classInternalAshStreamStaticBuffer->buffer->GetSize()) - 1) )
        {
            return ash::AshResult(false, "The cursor position is larger than the buffer size.");
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

        ash::AshBuffer* expandableBuffer;
        ash::AshSize expandableBufferCursor;
        bool hasErrorOccurred;
    };

    AshStreamExpandableExportBuffer::AshStreamExpandableExportBuffer()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshStreamExpandableExportBuffer);
        classInternalAshStreamExpandableExportBuffer->expandableBuffer = new ash::AshBuffer();
    }

    AshStreamExpandableExportBuffer::~AshStreamExpandableExportBuffer()
    {
        classInternalAshStreamExpandableExportBuffer->Reset();
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshStreamExpandableExportBuffer);
    }

    ash::AshBuffer* AshStreamExpandableExportBuffer::MakeCopyOfBuffer()
    {
        return classInternalAshStreamExpandableExportBuffer->expandableBuffer->DuplicateAndCopyBuffer();
    }
    
    AshResult AshStreamExpandableExportBuffer::ReadRawIntoPointer(void* Buffer, AshSize BufferSize)
    {
        return ash::AshResult(false, "ReadRawIntoPointer is not implemented because AshStreamExpandableExportBuffer is write only . ;-)");
    }

    AshResult AshStreamExpandableExportBuffer::WriteRawFromPointer(void* Buffer, AshSize BufferSize)
    {
        classInternalAshStreamExpandableExportBuffer->expandableBuffer->ExpandSize(BufferSize);
        
        for(AshSize i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(classInternalAshStreamExpandableExportBuffer->expandableBuffer->GetBytes())[classInternalAshStreamExpandableExportBuffer->expandableBufferCursor] = static_cast<unsigned char*>(Buffer)[i];
            ++classInternalAshStreamExpandableExportBuffer->expandableBufferCursor;
        }

        return ash::AshResult(true);
    }

    bool AshStreamExpandableExportBuffer::HasErrorOccurred() 
    { 
        return classInternalAshStreamExpandableExportBuffer->hasErrorOccurred; 
    }

    ash::AshSize AshStreamExpandableExportBuffer::GetCursorPosition() 
    { 
        return classInternalAshStreamExpandableExportBuffer->expandableBufferCursor; 
    }
}