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

        //! @brief Serializes the descriptor into an ash::Buffer and consumes the Descriptor object.
        static inline ash::CustomResult<ash::Buffer*> SerializeAndConsume(Base* Descriptor)
        {
            ash::CustomResult<ash::Buffer*> result = Serialize(Descriptor);
            delete Descriptor;
            return std::move(result);
        }

        //! @brief Deserializes the descriptor from an ash::Buffer and consumes the Descriptor (not the Buffer)
        static inline ash::Result DeserializeAndConsume(Base* Descriptor, ash::Buffer* Buffer)
        {
            ash::Result result = Deserialize(Descriptor, Buffer);
            delete Descriptor;
            return std::move(Descriptor);
        }
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_EASY_HPP
