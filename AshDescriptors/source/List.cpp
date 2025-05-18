#include "AshDescriptors/List.hpp"
#include "Ash/CRC32.hpp"

namespace ash::descriptors
{

    List::List(std::vector<Base*> List) : m_List(List)
    {
    }

    List::~List()
    {
        std::vector<Base*> list = m_List;
        m_List.clear();

        for (Base* currentIterator : list)
        {
            delete currentIterator;
        }
    }

    bool List::Serialize(DescriptorState& State)
    {
        dOrderSeed listRandomOrderSeed = rand();

        auto orderMap = sfGenerateOrderMapBySeed(listRandomOrderSeed, m_List.size());

        if (auto listRandomOrderSeedField = State.ConsumeCheckAndReturnPreviousLocationAs<dOrderSeed>())
        {
            *listRandomOrderSeedField = listRandomOrderSeed;
        }
        else
        {
            return false;
        }

        for (auto currentIteratorIndex : orderMap)
        {
            Base* currentIteratorItem = m_List[currentIteratorIndex.second];

            if (currentIteratorItem->Serialize(State) == false)
            {
                return false;
            }
        }

        return true;
    }

    bool List::Deserialize(DescriptorState& State)
    {
        dOrderSeed listRandomOrderSeed = 0;

        if (auto listRandomOrderSeedField = State.ConsumeCheckAndReturnPreviousLocationAs<dOrderSeed>())
        {
            listRandomOrderSeed = *listRandomOrderSeedField;
        }

        auto orderMap = sfGenerateOrderMapBySeed(listRandomOrderSeed, m_List.size());

        for (auto currentIteratorIndex : orderMap)
        {
            Base* currentIteratorItem = m_List[currentIteratorIndex.second];

            if (currentIteratorItem->Deserialize(State) == false)
            {
                return false;
            }
        }

        return true;
    }

    ash::Size List::GetRequiredSerializationLength()
    {
        ash::Size requiredSerializationLength = 0;

        // random order seed
        requiredSerializationLength += sizeof(dOrderSeed);

        for (auto currentIterator : m_List)
        {
            requiredSerializationLength += currentIterator->GetRequiredSerializationLength();
        }

        return requiredSerializationLength;
    }

    ash::Buffer* List::SerializeAndForget()
    {
        ash::Buffer* buffer = new ash::Buffer();

        if (buffer->AllocateBySize(this->GetRequiredSerializationLength()) == false)
        {
            return nullptr;
        }

        DescriptorState state = DescriptorState(buffer);

        if (this->Serialize(state) == false)
        {
            delete buffer;
            return nullptr;
        }

        return buffer;
    }

    bool List::DeserializeAndForget(ash::Buffer* Buffer, bool Deallocate)
    {
        if (Buffer == nullptr)
        {
            return false;
        }

        DescriptorState state = DescriptorState(Buffer);
        auto res = this->Deserialize(state);

        if (Deallocate == true)
        {
            delete Buffer;
        }

        return res;
    }

    List::dOrderMap List::sfGenerateOrderMapBySeed(ash::Size Seed, ash::Size Size, bool Reverse)
    {
        ash::CRC32State orderRandomizer = ash::CRC32State();
        dOrderMap orderMap = dOrderMap();
        std::vector<ash::Size> orderKeysLeft = {};

        // Populate keys
        for (ash::Size currentIndex = 0; currentIndex < Size; currentIndex++)
        {
            orderKeysLeft.push_back(currentIndex);
        }

        // Prepare Randomizer
        orderRandomizer.UpdateWithPrimitive(Seed);
        orderRandomizer.UpdateWithPrimitive(Size);

        // Populate Map

        for (ash::Size currentIndex = 0; currentIndex < Size; currentIndex++)
        {
            ash::Size currentOderKeyIndexChosen = ((orderRandomizer.GetValue() ^ Seed) % orderKeysLeft.size());
            ash::Size currentOderKeyValueChosen = orderKeysLeft.at(currentOderKeyIndexChosen);
            orderKeysLeft.erase(orderKeysLeft.begin() + currentOderKeyIndexChosen);

            ash::Size l = currentIndex, r = currentOderKeyValueChosen;

            if (Reverse == true)
            {
                std::swap(l, r);
            }

            orderMap[l] = r;

            orderRandomizer.UpdateWithPrimitive(l);
            orderRandomizer.UpdateWithPrimitive(r);
        }

        return std::move(orderMap);
    }

} // namespace ash::descriptors
