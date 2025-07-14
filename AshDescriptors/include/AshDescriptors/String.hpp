#ifndef ASHDESCRIPTORS_STRING_HPP
#define ASHDESCRIPTORS_STRING_HPP

#include "AshDescriptors/Buffer.hpp"

namespace ash::descriptors
{

    class String : public Base
    {
    public:
        enum class FilterType
        {
            NONE = 0,
            ALPHA_NUM = 1,
        };

        inline String(std::string* Buffer, ash::Size MaxSize = 0xFF, FilterType Filter = FilterType::NONE, SizeType SizeFieldType = SizeType::U8)
        {
            m_Buffer = Buffer;
            m_MaxSize = MaxSize;
            m_Filter = Filter;
            m_SizeFieldType = SizeFieldType;
        }

        bool Serialize(State& State) override;
        bool Deserialize(State& State) override;
        ash::Size GetRequiredSerializationLength() override;

    private:
        std::string* m_Buffer;
        ash::Size m_MaxSize;
        FilterType m_Filter;
        SizeType m_SizeFieldType;
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_STRING_HPP
