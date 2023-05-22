#pragma once
#include <Ash/AshStreamableObject.h>

namespace ash::objects
{

    class AshBufferPlug : public AshStreamableObject
    {
    public:
        // Dont forget to deallocate when using this.
        inline AshBufferPlug() {}
        AshBufferPlug(ash::AshBuffer* Buffer);
        inline ~AshBufferPlug() = default;

        inline bool SetBufferSizeLimit(ash::AshSize BufferSizeLimit) { bufferSizeLimit = BufferSizeLimit; return true; }

        inline ash::AshBuffer* GetBuffer() { return buffer; }
        inline ash::AshSize GetBufferSizeLimit() { return bufferSizeLimit; }

        ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE("AshBufferPlug");
    private:
        ash::AshBuffer* buffer;
        ash::AshSize bufferSizeLimit;
    };

}