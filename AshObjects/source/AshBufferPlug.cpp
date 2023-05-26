#include "AshObjects/AshBufferPlug.h"

/*
 * AshBufferPlug Recipe
 *
 * <AshSize : 8 : buffer length>
 * <unsigned char[] : 1 * buffer length : buffer data>
 */

namespace ash::objects
{

    static constexpr ash::AshSize smAshBufferPlugBufferSizeLimit = 0x1fff;

    AshBufferPlug::AshBufferPlug()
    {}

    AshBufferPlug::AshBufferPlug(ash::AshBuffer* Buffer):
        buffer(Buffer),
        bufferSizeLimit(smAshBufferPlugBufferSizeLimit)
    {}

    bool AshBufferPlug::Import(ash::AshStream* Stream)
    {
        AshSize bufferSize = Stream->Read<AshSize>();

        if(bufferSize > bufferSizeLimit)
        {
            return false;
        }

        if(bufferSize <= 0)
        {
            this->bufferEmpty = true;
            return true;
        }

        this->buffer = new ash::AshBuffer();
        this->buffer->AllocateSize(bufferSize);
        
        Stream->ReadRawIntoPointer(this->buffer->GetPointer(), bufferSize);

        return Stream->HasErrorOccurred() == false;
    }

    bool AshBufferPlug::Export(ash::AshStream* Stream)
    {
        Stream->Write<AshSize>(this->buffer->GetSize());

        if(this->buffer->GetSize() <= 0)
        {
            return Stream->HasErrorOccurred() == false;
        }
        
        Stream->WriteRawFromPointer(this->buffer->GetPointer(), this->buffer->GetSize());

        return Stream->HasErrorOccurred() == false;
    }

    AshSize AshBufferPlug::GetExportSize()
    {
        return (
            sizeof(AshSize) +
            this->buffer->GetSize()
        );
    }
}