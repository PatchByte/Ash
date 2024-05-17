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
        m_Buffer(nullptr),
        m_BufferEmpty(false),
        m_BufferSizeLimit(smAshDataBufferPlugBufferSizeLimit)
    {}

    AshDataBufferPlug::AshDataBufferPlug(ash::AshBuffer* Buffer):
        m_Buffer(Buffer),
        m_BufferEmpty(false),
        m_BufferSizeLimit(smAshDataBufferPlugBufferSizeLimit)
    {}

    AshDataBufferPlug::~AshDataBufferPlug()
    {
        if(m_Buffer)
        {
            delete m_Buffer;
            m_Buffer = nullptr;
        }
    }

    bool AshDataBufferPlug::Import(ash::AshStream* Stream)
    {
        if(this->m_Buffer != nullptr)
        {
            this->m_Buffer->ReleaseMemory();
            delete this->m_Buffer;
            this->m_Buffer = nullptr;
        }

        AshSize bufferSize = Stream->Read<AshSize>();

        if(bufferSize > m_BufferSizeLimit)
        {
            return false;
        }

        if(bufferSize <= 0)
        {
            this->m_BufferEmpty = true;
            return Stream->IsOkay();
        }

        this->m_Buffer = new ash::AshBuffer();
        this->m_Buffer->AllocateSize(bufferSize);
        
        Stream->ReadRawIntoPointer(this->m_Buffer->GetPointer(), bufferSize);

        return Stream->HasErrorOccurred() == false;
    }

    bool AshDataBufferPlug::Export(ash::AshStream* Stream)
    {
        if(this->m_Buffer == nullptr)
        {
            // By-pass
            Stream->Write<AshSize>(0);
            return true;
        }

        Stream->Write<AshSize>(this->m_Buffer->GetSize());

        if(this->m_Buffer->GetSize() <= 0)
        {
            this->m_BufferEmpty = true;
            return Stream->HasErrorOccurred() == false;
        }

        Stream->WriteRawFromPointer(this->m_Buffer->GetPointer(), this->m_Buffer->GetSize());

        return Stream->HasErrorOccurred() == false;
    }

}
