#ifndef ASHDESCRIPTORS_PRIMITIVE_HPP
#define ASHDESCRIPTORS_PRIMITIVE_HPP

#include "AshDescriptors/Base.hpp"

namespace ash::descriptors
{

    class Primitive : public Base
    {
    public:
        template <typename T>
        inline Primitive(T* Reference)
        {
            m_ReferenceDataBuffer = static_cast<void*>(Reference);
            m_ReferenceDataLength = sizeof(T);
        }

        bool Serialize(State& State) override;
        bool Deserialize(State& State) override;

        ash::Size GetRequiredSerializationLength() override
        {
            return m_ReferenceDataLength;
        }

    private:
        void* m_ReferenceDataBuffer;
        ash::Size m_ReferenceDataLength;
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_PRIMITIVE_HPP
