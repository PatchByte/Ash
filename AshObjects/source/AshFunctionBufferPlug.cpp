#include "AshObjects/AshBufferPlug.h"

namespace ash::objects
{
    AshFunctionBufferPlug::AshFunctionBufferPlug():
        m_ExportFunction([]() -> ash::AshBuffer* { return nullptr; }),
        m_ImportFunction([] (...) {}),
        m_CachedBuffer(nullptr)
    {}

    AshFunctionBufferPlug::~AshFunctionBufferPlug()
    {
        if(m_CachedBuffer != nullptr)
        {
            delete m_CachedBuffer;
            m_CachedBuffer = nullptr;
        }
    }

    bool AshFunctionBufferPlug::Import(ash::AshStream* Stream)
    {
        AshDataBufferPlug* dataBufferPlug = new AshDataBufferPlug();

        if(dataBufferPlug->Import(Stream) == false)
        {
            return false;
        }

        this->m_ImportFunction(dataBufferPlug->GetBuffer());

        delete dataBufferPlug;

        return Stream->HasErrorOccurred() == false;
    }

    bool AshFunctionBufferPlug::Export(ash::AshStream* Stream)
    {
        if(m_CachedBuffer == nullptr)
        {
            // @todo maybe change this in the future to -> return false;
            return true;
        }

        AshDataBufferPlug* dataBufferPlug = new AshDataBufferPlug(m_CachedBuffer);

        if(dataBufferPlug->Export(Stream) == false)
        {
            return false;
        }

        delete dataBufferPlug;

        return Stream->HasErrorOccurred() == false;
    }
}
