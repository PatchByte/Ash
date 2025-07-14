#include "AshDescriptors/State.hpp"

namespace ash::descriptors
{

    State::State(ash::Buffer* Buffer) : m_Buffer(Buffer), m_CurrentOffset(0), m_BytesLeft(m_Buffer->GetSize())
    {
    }

    bool State::HasBytesLeft(ash::Size Size)
    {
        return ((signed long long)m_BytesLeft - (signed long long)Size) >= 0;
    }

    bool State::ConsumeBytes(ash::Size Size)
    {
        if (HasBytesLeft(Size))
        {
            m_CurrentOffset += Size;
            m_BytesLeft -= Size;

            return true;
        }

        return false;
    }

    void* State::ConsumeCheckAndReturnPreviousLocation(ash::Size Size)
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
