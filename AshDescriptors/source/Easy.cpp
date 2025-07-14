#include "AshDescriptors/Easy.hpp"
#include "Ash/Buffer.hpp"
#include "Ash/Result.hpp"
#include "AshDescriptors/State.hpp"

namespace ash::descriptors
{

    ash::CustomResult<ash::Buffer*> Easy::Serialize(Base* Descriptor)
    {
        Buffer* buffer = new Buffer();

        if (buffer->AllocateBySize(Descriptor->GetRequiredSerializationLength()) == false)
        {
            return ash::CustomResult<ash::Buffer*>(false, "Failed to allocate");
        }

        State state = State(buffer);

        if (Descriptor->Serialize(state) == false)
        {
            return ash::CustomResult<ash::Buffer*>(false, "Failed to serialize");
        }

        return ash::CustomResult<ash::Buffer*>(true).Attach(buffer);
    }

    ash::Result Easy::Deserialize(Base* Descriptor, ash::Buffer* Buffer)
    {
        State state = State(Buffer);

        if (Descriptor->Deserialize(state) == false)
        {
            return ash::Result(false, "Failed to deserialize");
        }

        return ash::Result(true);
    }

} // namespace ash::descriptors
