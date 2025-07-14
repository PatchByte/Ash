#ifndef ASHDESCRIPTORS_EASY_HPP
#define ASHDESCRIPTORS_EASY_HPP

#include "Ash/Result.hpp"
#include "AshDescriptors/Base.hpp"
#include <Ash/Buffer.hpp>

namespace ash::descriptors
{

    class Easy
    {
    public:
        static ash::CustomResult<ash::Buffer*> Serialize(Base* Descriptor);
        static ash::Result Deserialize(Base* Descriptor, ash::Buffer* Buffer);
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_EASY_HPP
