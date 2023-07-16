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
        virtual ~AshStream() = default;

        template<typename T>
        inline T* Read(T* Buffer)
        {
            static_assert(std::is_fundamental<T>::value == true, "You can only use primitive types.");
            
            auto r = ReadRawIntoPointer(Buffer, sizeof(T));
            return r.WasSuccessful() ? Buffer : nullptr;
        }

        template<typename T>
        inline T Read()
        {
            static_assert(std::is_fundamental<T>::value == true, "You can only use primitive types.");

            T Data = { 0 };

            auto r = Read(&Data);
            return Data;
        }

        template<typename T>
        inline bool Write(T Data)
        {
            static_assert(std::is_fundamental<T>::value == true, "You can only use primitive types.");

            auto r = Write(&Data);
            return r != nullptr;
        }

        template<typename T>
        inline T* Write(T* Buffer)
        {
            static_assert(std::is_fundamental<T>::value == true, "You can only use primitive types.");

            auto r = WriteRawFromPointer(Buffer, sizeof(T));
            return r.WasSuccessful() ? Buffer : nullptr;
        }

        virtual AshResult ReadRawIntoPointer(void* Buffer, AshSize BufferSize) { return ash::AshResult(false, "Non implemented function."); }
        virtual AshResult WriteRawFromPointer(void* Buffer, AshSize BufferSize) { return ash::AshResult(false, "Non implemented function."); }

        virtual bool IsReadOnly() { return this->GetStreamMode() == AshStreamMode::READ; }
        virtual bool IsWriteOnly() { return this->GetStreamMode() == AshStreamMode::WRITE; }

        virtual bool IsEndOfStream() { return true; }
        virtual bool HasErrorOccurred() { return true; }
        virtual bool IsOkay() { return HasErrorOccurred() == false; }

        virtual AshStreamMode GetStreamMode() { return AshStreamMode::INVALID; }
    };

    class AshStreamStaticBuffer : public AshStream
    {
    public:
        AshStreamStaticBuffer(ash::AshBuffer* StaticBuffer, AshStreamMode StaticBufferMode);
        ~AshStreamStaticBuffer();

        virtual AshStreamStaticBuffer* DeallocateBufferAfterUse();

        AshResult ReadRawIntoPointer(void* Buffer, AshSize BufferSize);
        AshResult WriteRawFromPointer(void* Buffer, AshSize BufferSize);

        bool IsEndOfStream();
        bool HasErrorOccurred();

        AshStreamMode GetStreamMode();
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshStreamStaticBuffer);
    };

    class AshStreamExpandableExportBuffer : public AshStream
    {
    public:
        AshStreamExpandableExportBuffer();
        ~AshStreamExpandableExportBuffer();

        virtual ash::AshBuffer* MakeCopyOfBuffer();

        AshResult ReadRawIntoPointer(void* Buffer, AshSize BufferSize);
        AshResult WriteRawFromPointer(void* Buffer, AshSize BufferSize);

        bool IsEndOfStream() { return false; }
        bool HasErrorOccurred();

        AshStreamMode GetStreamMode() { return AshStreamMode::WRITE; }
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshStreamExpandableExportBuffer);
    };

}