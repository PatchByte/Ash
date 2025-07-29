#include "AshDescriptors/Easy.hpp"
#include "Ash/Buffer.hpp"
#include "Ash/Result.hpp"
#include "Ash/Serializer.hpp"
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

    ash::CustomResult<std::string> Easy::SerializeAndConsumeToText(Base* Descriptor)
    {
        if (auto res = SerializeAndConsume(Descriptor); res.HasError())
        {
            return ash::CustomResult<std::string>(false, "Failed to serialize. " + res.GetMessage());
        }
        else
        {
            ash::Buffer* buffer = res.Get();
            std::string text = ash::TextSerializer::Encode(buffer);

            delete buffer;
            buffer = nullptr;

            return ash::CustomResult<std::string>(true).Attach(text);
        }
    }

    ash::Result Easy::DeserializeAndConsumeFromText(Base* Descriptor, std::string Text)
    {
        if (ash::Buffer* buffer = ash::TextSerializer::Decode(Text); buffer == nullptr)
        {
            return ash::Result(false, "Failed to decode");
        }
        else
        {
            auto res = DeserializeAndConsume(Descriptor, buffer);

            delete buffer;
            buffer = nullptr;

            return std::move(res);
        }
    }

} // namespace ash::descriptors
