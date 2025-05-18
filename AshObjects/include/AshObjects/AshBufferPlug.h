#pragma once
#include "Ash/AshBuffer.h"
#include <Ash/AshStreamableObject.h>
#include <functional>
#include <utility>

namespace ash::objects
{

    class AshDataBufferPlug : public StreamableObject
    {
    public:
        // Dont forget to deallocate when using this.
        AshDataBufferPlug();
        AshDataBufferPlug(ash::Buffer* Buffer);
        ~AshDataBufferPlug();

        inline bool SetBufferSizeLimit(ash::Size BufferSizeLimit) { m_BufferSizeLimit = BufferSizeLimit; return true; }

        inline ash::Buffer* GetBuffer() { return m_Buffer; }
        inline ash::Size GetBufferSizeLimit() { return m_BufferSizeLimit; }

        inline ash::Buffer* ConsumeBuffer()
        {
            ash::Buffer* v = m_Buffer;
            m_Buffer = nullptr;
            return v;
        }

        inline bool IsBufferEmpty() { return m_BufferEmpty; }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshDataBufferPlug");
    private:
        ash::Buffer* m_Buffer;
        ash::Size m_BufferSizeLimit;
        bool m_BufferEmpty;
    };

    class AshFunctionBufferPlug : public StreamableObject
    {
    public:
        using ExportFunctionDelegate = std::function<ash::Buffer*()>;
        using ImportFunctionDelegate = std::function<void(ash::Buffer*)>;

        AshFunctionBufferPlug();
        ~AshFunctionBufferPlug();

        inline bool SetExportFunction(ExportFunctionDelegate ExportFunction) { m_ExportFunction = std::move(ExportFunction); return true; }
        inline bool SetImportFunction(ImportFunctionDelegate ImportFunction) { m_ImportFunction = std::move(ImportFunction); return true; }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshFunctionBufferPlug");
    private:
        ExportFunctionDelegate m_ExportFunction;
        ImportFunctionDelegate m_ImportFunction;
        ash::Buffer* m_CachedBuffer;
    };

}
