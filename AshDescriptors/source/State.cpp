#include "AshDescriptors/State.hpp"

namespace ash::descriptors
{

    DescriptorState::DescriptorState(ash::Buffer* Buffer) : m_Buffer(Buffer), m_CurrentOffset(0), m_BytesLeft(m_Buffer->GetSize())
    {
    }

    bool DescriptorState::HasBytesLeft(ash::Size Size)
    {
        return ((signed long long)m_BytesLeft - (signed long long)Size) >= 0;
    }

    bool DescriptorState::ConsumeBytes(ash::Size Size)
    {
        if (HasBytesLeft(Size))
        {
            m_CurrentOffset += Size;
            m_BytesLeft -= Size;

            return true;
        }

        return false;
    }

    void* DescriptorState::ConsumeCheckAndReturnPreviousLocation(ash::Size Size)
    {
        if (HasBytesLeft(Size) == false)
        {
            return nullptr;
        }

        void* location = this->GetCurrentOffsetLocation();

        ConsumeBytes(Size);

        return location;
    }

} // namespace ash::descriptors
