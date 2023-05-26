#include "AshObjects/AshBufferPlug.h"

/*
 * AshDataBufferPlug Recipe
 *
 * <AshSize : 8 : buffer length>
 * <unsigned char[] : 1 * buffer length : buffer data>
 */

namespace ash::objects
{

    static constexpr ash::AshSize smAshDataBufferPlugBufferSizeLimit = 0x1fff;

    AshDataBufferPlug::AshDataBufferPlug():
        buffer(nullptr),
        bufferEmpty(false),
        bufferSizeLimit(smAshDataBufferPlugBufferSizeLimit)
    {}

    AshDataBufferPlug::AshDataBufferPlug(ash::AshBuffer* Buffer):
        buffer(nullptr),
        bufferEmpty(false),
        bufferSizeLimit(smAshDataBufferPlugBufferSizeLimit)
    {}

    bool AshDataBufferPlug::Import(ash::AshStream* Stream)
    {
        if(this->buffer != nullptr)
        {
            this->buffer->ReleaseMemory();
            delete this->buffer;
            this->buffer = nullptr;
        }

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

    bool AshDataBufferPlug::Export(ash::AshStream* Stream)
    {
        if(this->buffer == nullptr)
        {
            // By-pass
            Stream->Write<AshSize>(0);
            return true;
        }

        Stream->Write<AshSize>(this->buffer->GetSize());

        if(this->buffer->GetSize() <= 0)
        {
            this->bufferEmpty = true;
            return Stream->HasErrorOccurred() == false;
        }

        Stream->WriteRawFromPointer(this->buffer->GetPointer(), this->buffer->GetSize());

        return Stream->HasErrorOccurred() == false;
    }

    AshSize AshDataBufferPlug::GetExportSize()
    {
        return (
            sizeof(AshSize) +
            this->buffer->GetSize()
        );
    }
}