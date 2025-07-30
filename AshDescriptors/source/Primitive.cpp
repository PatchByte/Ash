#include "AshDescriptors/Primitive.hpp"
#include <cstring>

namespace ash::descriptors
{

    bool Primitive::Serialize(State& State)
    {
        if (m_ReferenceDataBuffer == nullptr)
        {
            return State.ConsumeBytes(m_ReferenceDataLength);
        }

        if (auto location = State.ConsumeCheckAndReturnPreviousLocation(m_ReferenceDataLength))
        {
            memcpy(location, m_ReferenceDataBuffer, m_ReferenceDataLength);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool Primitive::Deserialize(State& State)
    {
        if (m_ReferenceDataBuffer == nullptr)
        {
            return State.ConsumeBytes(m_ReferenceDataLength);
        }

        if (auto location = State.ConsumeCheckAndReturnPreviousLocation(m_ReferenceDataLength))
        {
            memcpy(m_ReferenceDataBuffer, location, m_ReferenceDataLength);
        }
        else
        {
            return false;
        }

        return true;
    }

} // namespace ash::descriptors
