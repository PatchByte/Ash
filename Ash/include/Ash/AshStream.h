#pragma once
#include "AshClass.h"
#include "AshBuffer.h"
#include "AshResult.h"

namespace ash
{
    enum class AshStreamMode : char
    {
        INVALID,
        READ,
        WRITE
    };

    class AshStream
    {
    public:
        AshStream(AshBuffer* Buffer, AshStreamMode BufferMode);
        virtual ~AshStream();

        // If you set this the AshBuffer* will be deallocate after use.
        virtual bool SetSingleUseBuffer();

        template<typename T>
        inline T* Read(T* Buffer)
        {
            auto r = ReadRawIntoPointer(Buffer, sizeof(T));
            return r.WasSuccessful() ? Buffer : nullptr;
        }

        template<typename T>
        inline T Read()
        {
            T Data = {};
            auto r = Read(&Data);
            return Data;
        }

        template<typename T>
        inline bool Write(T Data)
        {
            auto r = Write(&Data);
            return r != nullptr;
        }

        template<typename T>
        inline T* Write(T* Buffer)
        {
            auto r = WriteRawFromPointer(Buffer, sizeof(T));
            return r.WasSuccessful() ? Buffer : nullptr;
        }

        virtual AshResult ReadRawIntoPointer(void* Buffer, AshSize BufferSize);
        virtual AshResult WriteRawFromPointer(void* Buffer, AshSize BufferSize);

        virtual bool IsReadOnly();
        virtual bool IsWriteOnly();
        virtual bool IsEndOfFile();
        virtual bool HasErrorOccurred();

        virtual AshStreamMode GetStreamMode();
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshStream);
    };

}