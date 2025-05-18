#pragma once
#include "AshClass.h"
#include "AshBuffer.h"
#include "AshResult.h"

namespace ash
{
    enum class StreamMode : char
    {
        INVALID,
        READ,
        WRITE
    };

    class Stream
    {
    public:
        virtual ~Stream() = default;

        template<typename T>
        inline T* ReadToPointer(T* Buffer)
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

            auto r = ReadToPointer(&Data);
            return Data;
        }

        template<typename T>
        inline T* WriteFromPointer(T* Buffer)
        {
            static_assert(std::is_fundamental<T>::value == true, "You can only use primitive types.");

            auto r = WriteRawFromPointer(Buffer, sizeof(T));
            return r.WasSuccessful() ? Buffer : nullptr;
        }

        template<typename T>
        inline bool Write(T Data)
        {
            static_assert(std::is_fundamental<T>::value == true, "You can only use primitive types.");

            auto r = WriteFromPointer(&Data);
            return r != nullptr;
        }

        virtual Result ReadRawIntoPointer(void* Buffer, Size BufferSize) { return ash::Result(false, "Non implemented function."); }
        virtual Result WriteRawFromPointer(void* Buffer, Size BufferSize) { return ash::Result(false, "Non implemented function."); }

        virtual bool IsReadOnly() { return this->GetStreamMode() == StreamMode::READ; }
        virtual bool IsWriteOnly() { return this->GetStreamMode() == StreamMode::WRITE; }

        virtual bool IsEndOfStream() { return true; }
        virtual bool HasErrorOccurred() { return true; }
        virtual bool IsOkay() { return HasErrorOccurred() == false; }

        virtual StreamMode GetStreamMode() { return StreamMode::INVALID; }

        virtual ash::Size GetCursorPosition() { return 0; }
        virtual ash::Result SetCursorPosition(ash::Size CursorPosition) { return ash::Result(false, "Non implemented function."); }

        virtual ash::Result SkipSize(ash::Size Size) { return this->SetCursorPosition(this->GetCursorPosition() + Size); }
        virtual bool FillSize(ash::Size Size, unsigned char Value = 0x00) { for(ash::Size currentSizeIndex = 0; currentSizeIndex < Size; currentSizeIndex++) { this->Write<unsigned char>(Value); } return this->IsOkay(); }
    };

    class StreamStaticBuffer : public Stream
    {
    public:
        StreamStaticBuffer(ash::Buffer* StaticBuffer, StreamMode StaticBufferMode);
        ~StreamStaticBuffer();

        virtual StreamStaticBuffer* DeallocateBufferAfterUse();

        Result ReadRawIntoPointer(void* Buffer, Size BufferSize);
        Result WriteRawFromPointer(void* Buffer, Size BufferSize);

        bool IsEndOfStream();
        bool HasErrorOccurred();

        StreamMode GetStreamMode();

        ash::Size GetCursorPosition();
        ash::Result SetCursorPosition(ash::Size CursorPosition);
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshStreamStaticBuffer);
    };

    class StreamExpandableExportBuffer : public Stream
    {
    public:
        StreamExpandableExportBuffer();
        ~StreamExpandableExportBuffer();

        virtual ash::Buffer* MakeCopyOfBuffer();

        Result ReadRawIntoPointer(void* Buffer, Size BufferSize);
        Result WriteRawFromPointer(void* Buffer, Size BufferSize);

        bool IsEndOfStream() { return false; }
        bool HasErrorOccurred();

        StreamMode GetStreamMode() { return StreamMode::WRITE; }

        ash::Size GetCursorPosition();
        ash::Result SetCursorPosition(ash::Size CursorPosition) { return ash::Result(false, "Not supported in AshStreamExpandableExportBuffer."); }
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshStreamExpandableExportBuffer);
    };

}
