#pragma once
#include <Ash/AshStreamableObject.h>
#include <functional>

namespace ash::objects
{

    class AshDataBufferPlug : public AshStreamableObject
    {
    public:
        // Dont forget to deallocate when using this.
        AshDataBufferPlug();
        AshDataBufferPlug(ash::AshBuffer* Buffer);
        ~AshDataBufferPlug() = default;

        inline bool SetBufferSizeLimit(ash::AshSize BufferSizeLimit) { bufferSizeLimit = BufferSizeLimit; return true; }

        inline ash::AshBuffer* GetBuffer() { return buffer; }
        inline ash::AshSize GetBufferSizeLimit() { return bufferSizeLimit; }
        
        inline bool IsBufferEmpty() { return bufferEmpty; }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshDataBufferPlug");
    private:
        ash::AshBuffer* buffer;
        ash::AshSize bufferSizeLimit;
        bool bufferEmpty;
    };

    class AshFunctionBufferPlug : public AshStreamableObject
    {
    public:
        using ExportFunctionDelegate = std::function<ash::AshBuffer*()>;
        using ImportFunctionDelegate = std::function<void(ash::AshBuffer*)>;

        AshFunctionBufferPlug();
        ~AshFunctionBufferPlug();

        inline bool SetExportFunction(ExportFunctionDelegate ExportFunction) { exportFunction = ExportFunction; return true; }
        inline bool SetImportFunction(ImportFunctionDelegate ImportFunction) { importFunction = ImportFunction; return true; }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshFunctionBufferPlug");
    private:
        ExportFunctionDelegate exportFunction;
        ImportFunctionDelegate importFunction;
        ash::AshBuffer* cachedBuffer;
    };

}