#ifndef ASHDESCRIPTORS_LIST_HPP
#define ASHDESCRIPTORS_LIST_HPP

#include "Ash/Buffer.hpp"
#include "AshDescriptors/Base.hpp"
#include <map>

namespace ash::descriptors
{

    class List : public Base
    {
    public:
        //! @todo rename this stupid thing
        using dOrderMap = std::map<ash::Size, ash::Size>;
        using dOrderSeed = unsigned short;

        //! @warning Will consume the pointers in the list
        List(std::vector<Base*> List);
        ~List() override;

        bool Serialize(State& State) override;
        bool Deserialize(State& State) override;
        ash::Size GetRequiredSerializationLength() override;

        ash::Buffer* SerializeAndForget();
        bool DeserializeAndForget(ash::Buffer* Buffer, bool Deallocate = true);

        static dOrderMap sfGenerateOrderMapBySeed(ash::Size Seed, ash::Size Size, bool Reverse = false);

    private:
        std::vector<Base*> m_List;
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_LIST_HPP
