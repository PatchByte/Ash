#include "AshDescriptors/Buffer.hpp"
#include <cstring>

namespace ash::descriptors
{

    bool Buffer::Serialize(State& State)
    {
        if (auto bufferSizeField = State.ConsumeCheckAndReturnPreviousLocation(static_cast<unsigned int>(m_SizeFieldType)))
        {
            //! @todo Should be cast down to int, but lets do not care about that for now
            ash::Size bufferSize = m_Buffer->GetSize();

            memcpy(bufferSizeField, &bufferSize, static_cast<unsigned int>(m_SizeFieldType));
        }
        else
        {
            return false;
        }

        if (auto bufferDataField = State.ConsumeCheckAndReturnPreviousLocation(m_Buffer->GetSize()))
        {
            memcpy(bufferDataField, m_Buffer->GetPointer(), m_Buffer->GetSize());
        }
        else
        {
            return false;
        }

        return true;
    }

    bool Buffer::Deserialize(State& State)
    {
        if (auto bufferSizeField = State.ConsumeCheckAndReturnPreviousLocation(static_cast<unsigned int>(m_SizeFieldType)))
        {
            ash::Size bufferSize = 0;

            memcpy(&bufferSize, bufferSizeField, static_cast<unsigned int>(m_SizeFieldType));

            if (bufferSize > m_MaxSize)
            {
                return false;
            }

            if (bufferSize)
            {
                m_Buffer->AllocateBySize(bufferSize);
            }
            else
            {
                m_Buffer->Release();
            }
        }
        else
        {
            return false;
        }

        if (m_Buffer->IsAllocated())
        {
            if (auto bufferDataField = State.ConsumeCheckAndReturnPreviousLocation(m_Buffer->GetSize()))
            {
                memcpy(m_Buffer->GetPointer(), bufferDataField, m_Buffer->GetSize());
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    ash::Size Buffer::GetRequiredSerializationLength()
    {
        return static_cast<unsigned int>(m_SizeFieldType) + m_Buffer->GetSize();
    }

} // namespace ash::descriptors
