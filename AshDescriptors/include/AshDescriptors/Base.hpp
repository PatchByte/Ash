#ifndef ASHDESCRIPTORS_BASE_HPP
#define ASHDESCRIPTORS_BASE_HPP

#include "AshDescriptors/State.hpp"

namespace ash::descriptors
{

    class Base
    {
    public:
        virtual ~Base() = default;
        virtual bool Serialize(DescriptorState& State) = 0;
        virtual bool Deserialize(DescriptorState& State) = 0;
        virtual ash::Size GetRequiredSerializationLength() = 0;
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_BASE_HPP
