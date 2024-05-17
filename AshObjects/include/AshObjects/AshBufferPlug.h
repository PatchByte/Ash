#pragma once
#include "Ash/AshBuffer.h"
#include <Ash/AshStreamableObject.h>
#include <functional>
#include <utility>

namespace ash::objects
{

    class AshDataBufferPlug : public AshStreamableObject
    {
    public:
        // Dont forget to deallocate when using this.
        AshDataBufferPlug();
        AshDataBufferPlug(ash::AshBuffer* Buffer);
        ~AshDataBufferPlug();

        inline bool SetBufferSizeLimit(ash::AshSize BufferSizeLimit) { m_BufferSizeLimit = BufferSizeLimit; return true; }

        inline ash::AshBuffer* GetBuffer() { return m_Buffer; }
        inline ash::AshSize GetBufferSizeLimit() { return m_BufferSizeLimit; }

        inline ash::AshBuffer* ConsumeBuffer()
        {
            ash::AshBuffer* v = m_Buffer;
            m_Buffer = nullptr;
            return v;
        }
        
        inline bool IsBufferEmpty() { return m_BufferEmpty; }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshDataBufferPlug");
    private:
        ash::AshBuffer* m_Buffer;
        ash::AshSize m_BufferSizeLimit;
        bool m_BufferEmpty;
    };

    class AshFunctionBufferPlug : public AshStreamableObject
    {
    public:
        using ExportFunctionDelegate = std::function<ash::AshBuffer*()>;
        using ImportFunctionDelegate = std::function<void(ash::AshBuffer*)>;

        AshFunctionBufferPlug();
        ~AshFunctionBufferPlug();

        inline bool SetExportFunction(ExportFunctionDelegate ExportFunction) { m_ExportFunction = std::move(ExportFunction); return true; }
        inline bool SetImportFunction(ImportFunctionDelegate ImportFunction) { m_ImportFunction = std::move(ImportFunction); return true; }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshFunctionBufferPlug");
    private:
        ExportFunctionDelegate m_ExportFunction;
        ImportFunctionDelegate m_ImportFunction;
        ash::AshBuffer* m_CachedBuffer;
    };

}
