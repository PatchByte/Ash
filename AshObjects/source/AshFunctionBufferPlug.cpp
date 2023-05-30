#include "AshObjects/AshBufferPlug.h"

namespace ash::objects
{
    AshFunctionBufferPlug::AshFunctionBufferPlug():
        exportFunction([]() -> ash::AshBuffer* { return nullptr; }),
        importFunction([] (...) {}),
        cachedBuffer(nullptr)
    {}

    AshFunctionBufferPlug::~AshFunctionBufferPlug()
    {
        if(cachedBuffer != nullptr)
        {
            delete cachedBuffer;
            cachedBuffer = nullptr;
        }
    }

    bool AshFunctionBufferPlug::Import(ash::AshStream* Stream)
    {
        AshDataBufferPlug* dataBufferPlug = new AshDataBufferPlug();

        if(dataBufferPlug->Import(Stream) == false)
        {
            return false;
        }

        this->importFunction(dataBufferPlug->GetBuffer());

        delete dataBufferPlug;

        return Stream->HasErrorOccurred() == false;
    }

    bool AshFunctionBufferPlug::Export(ash::AshStream* Stream)
    {
        if(cachedBuffer == nullptr)
        {
            // @todo maybe change this in the future to -> return false;
            return true;
        }

        AshDataBufferPlug* dataBufferPlug = new AshDataBufferPlug(cachedBuffer);

        if(dataBufferPlug->Export(Stream) == false)
        {
            return false;
        }

        delete dataBufferPlug;

        return Stream->HasErrorOccurred() == false;
    }

    AshSize AshFunctionBufferPlug::GetExportSize()
    {
        if(cachedBuffer != nullptr)
        {
            delete cachedBuffer;
            cachedBuffer = nullptr;
        }

        cachedBuffer = exportFunction();

        return (cachedBuffer != nullptr) ? cachedBuffer->GetSize() : 0;
    }

}