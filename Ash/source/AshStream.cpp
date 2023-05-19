#include "Ash/AshStream.h"

namespace ash
{

    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(AshStream)
    {
    public:
        void Reset()
        {
            buffer = nullptr;
            bufferMode = AshStreamMode::INVALID;
            bufferCursor = 0;
            hasErrorOccurred = false;
        }

        AshBuffer* buffer;
        AshStreamMode bufferMode;
        AshSize bufferCursor;

        bool hasErrorOccurred;
    };

    AshStream::AshStream(AshBuffer* Buffer, AshStreamMode BufferMode)
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(AshStream);
        classInternalAshStream->Reset();
        classInternalAshStream->buffer = Buffer;
        classInternalAshStream->bufferMode = BufferMode;
    }

    AshStream::~AshStream()
    {
        ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(AshStream);
    }

    AshResult AshStream::ReadRawIntoPointer(void* Buffer, AshSize BufferSize)
    {
        if(IsReadOnly() == false)
        {
            return AshResult(false, "Stream is write only.");
        }

        if(IsEndOfFile())
        {
            classInternalAshStream->hasErrorOccurred = true;
            return AshResult(false, "Stream is eof.");
        }

        if((classInternalAshStream->bufferCursor + BufferSize) > classInternalAshStream->buffer->GetSize())
        {
            classInternalAshStream->hasErrorOccurred = true;
            return AshResult(false, "Size is overlapping the eof.");
        }

        for(AshSize i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(Buffer)[i] = static_cast<unsigned char*>(classInternalAshStream->buffer->GetBytes())[classInternalAshStream->bufferCursor];
            ++classInternalAshStream->bufferCursor;
        }

        return AshResult(true);
    }

    AshResult AshStream::WriteRawFromPointer(void* Buffer, AshSize BufferSize)
    {
        if(IsWriteOnly() == false)
        {
            return AshResult(false, "Stream is read only.");
        }

        if(IsEndOfFile())
        {
            classInternalAshStream->hasErrorOccurred = true;
            return AshResult(false, "Stream is eof.");
        }

        if((classInternalAshStream->bufferCursor + BufferSize) > classInternalAshStream->buffer->GetSize())
        {
            classInternalAshStream->hasErrorOccurred = true;
            return AshResult(false, "Size is overlapping the eof.");
        }

        for(AshSize i = 0; i < BufferSize; i++)
        {
            static_cast<unsigned char*>(classInternalAshStream->buffer->GetBytes())[classInternalAshStream->bufferCursor] = static_cast<unsigned char*>(Buffer)[i];
            ++classInternalAshStream->bufferCursor;
        }

        return AshResult(true);
    }

    bool AshStream::IsReadOnly()
    {
        return classInternalAshStream->bufferMode == AshStreamMode::READ;
    }

    bool AshStream::IsWriteOnly()
    {
        return classInternalAshStream->bufferMode == AshStreamMode::WRITE;
    }

    bool AshStream::IsEndOfFile()
    {
        return classInternalAshStream->bufferCursor >= classInternalAshStream->buffer->GetSize();
    }

    bool AshStream::HasErrorOccurred()
    {
        return classInternalAshStream->hasErrorOccurred;
    }

    AshStreamMode AshStream::GetStreamMode()
    {
        return classInternalAshStream->bufferMode;
    }

}