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

        virtual AshResult ReadRawIntoPointer(void* Buffer, AshSize BufferSize) { return ash::AshResult(false, "Non implemented function."); }
        virtual AshResult WriteRawFromPointer(void* Buffer, AshSize BufferSize) { return ash::AshResult(false, "Non implemented function."); }

        virtual bool IsReadOnly() { return this->GetStreamMode() == AshStreamMode::READ; }
        virtual bool IsWriteOnly() { return this->GetStreamMode() == AshStreamMode::WRITE; }

        virtual bool IsEndOfStream() { return true; }
        virtual bool HasErrorOccurred() { return true; }
        virtual bool IsOkay() { return HasErrorOccurred() == false; }

        virtual AshStreamMode GetStreamMode() { return AshStreamMode::INVALID; }

        virtual ash::AshSize GetCursorPosition() { return 0; }
        virtual ash::AshResult SetCursorPosition(ash::AshSize CursorPosition) { return ash::AshResult(false, "Non implemented function."); }
        
        virtual ash::AshResult SkipSize(ash::AshSize Size) { return this->SetCursorPosition(this->GetCursorPosition() + Size); }
        virtual bool FillSize(ash::AshSize Size, unsigned char Value = 0x00) { for(ash::AshSize currentSizeIndex = 0; currentSizeIndex < Size; currentSizeIndex++) { this->Write<unsigned char>(Value); } return this->IsOkay(); }
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

        ash::AshSize GetCursorPosition();
        ash::AshResult SetCursorPosition(ash::AshSize CursorPosition);
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

        ash::AshSize GetCursorPosition();
        ash::AshResult SetCursorPosition(ash::AshSize CursorPosition) { return ash::AshResult(false, "Not supported in AshStreamExpandableExportBuffer."); }
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshStreamExpandableExportBuffer);
    };

}