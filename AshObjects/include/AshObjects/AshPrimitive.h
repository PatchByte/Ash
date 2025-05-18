#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>

namespace ash::objects
{

    template<typename T>
    class AshPrimitive : public StreamableObject
    {
    public:
        AshPrimitive(T DataValue):
            m_DataValue(DataValue)
        {}

        AshPrimitive():
            m_DataValue()
        {}

        bool Import(Stream* Stream) override
        {
            m_DataValue = Stream->Read<T>();
            return Stream->HasErrorOccurred() == false;
        }

        bool Export(Stream* Stream) override
        {
            Stream->Write<T>(m_DataValue);
            return Stream->HasErrorOccurred() == false;
        }

        T GetDataValue()
        {
            return m_DataValue;
        }
    private:
        T m_DataValue;

        static_assert(std::is_fundamental<T>::value == true, "You can only use primitives");
    };

}
