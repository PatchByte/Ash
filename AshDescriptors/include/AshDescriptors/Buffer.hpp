#ifndef ASHDESCRIPTORS_BUFFER_HPP
#define ASHDESCRIPTORS_BUFFER_HPP

#include "Ash/Buffer.hpp"
#include "AshDescriptors/Base.hpp"

namespace ash::descriptors
{

    enum class SizeType : unsigned int
    {
        U8 = 1,
        U16 = 2,
        U32 = 4
    };

    class Buffer : public Base
    {
    public:
        inline Buffer(ash::Buffer* Buffer, ash::Size MaxSize = 0xFF, SizeType SizeFieldType = SizeType::U8)
        {
            m_Buffer = Buffer;
            m_MaxSize = MaxSize;
            m_SizeFieldType = SizeFieldType;
        }

        bool Serialize(State& State) override;
        bool Deserialize(State& State) override;
        ash::Size GetRequiredSerializationLength() override;

    private:
        ash::Buffer* m_Buffer;
        ash::Size m_MaxSize;
        SizeType m_SizeFieldType;
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_BUFFER_HPP
