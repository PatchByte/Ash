#include "AshDescriptors/String.hpp"
#include <cstring>

namespace ash::descriptors
{

    bool String::Serialize(State& State)
    {
        if (auto bufferSizeField = State.ConsumeCheckAndReturnPreviousLocation(static_cast<unsigned int>(m_SizeFieldType)))
        {
            //! @todo Should be cast down to int, but lets do not care about that for now
            ash::Size bufferSize = m_Buffer->size();

            memcpy(bufferSizeField, &bufferSize, static_cast<unsigned int>(m_SizeFieldType));
        }
        else
        {
            return false;
        }

        if (m_Buffer->size() > 0)
        {
            if (auto bufferDataField = State.ConsumeCheckAndReturnPreviousLocation(m_Buffer->size()))
            {
                memcpy(bufferDataField, m_Buffer->data(), m_Buffer->size());
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    bool String::Deserialize(State& State)
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
                m_Buffer->resize(bufferSize);
            }
            else
            {
                m_Buffer->clear();
            }
        }
        else
        {
            return false;
        }

        if (m_Buffer->size() > 0)
        {
            if (auto bufferDataField = State.ConsumeCheckAndReturnPreviousLocation(m_Buffer->size()))
            {
                memcpy(m_Buffer->data(), bufferDataField, m_Buffer->size());

                for (char currentCharacter : *m_Buffer)
                {
                    switch (m_Filter)
                    {
                    default:
                        break;
                    case FilterType::ALPHA_NUM:
                        if (std::isalnum(currentCharacter) == false)
                        {
                            m_Buffer->clear();
                            return false;
                        }
                    }
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    ash::Size String::GetRequiredSerializationLength()
    {
        return static_cast<unsigned int>(m_SizeFieldType) + m_Buffer->size();
    }

} // namespace ash::descriptors
