#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>

namespace ash::objects
{
    
    template<typename T>
    class AshPrimitive : public AshStreamableObject
    {
    public:
        AshPrimitive(T DataValue):
            dataValue(DataValue)
        {}

        AshPrimitive():
            dataValue()
        {}

        const char* GetName() override
        { 
            #if ASH_OBJECTS_ERASE_NAMES
            return "";
            #else
            static std::string name = std::string("AshPrimitive").append(typeid(T).name());
            return name.data(); 
            #endif
        }

        bool Import(AshStream* Stream) override
        {
            dataValue = Stream->Read<T>();
            return Stream->HasErrorOccurred() == false;
        }
        
        bool Export(AshStream* Stream) override
        { 
            Stream->Write<T>(dataValue);
            return Stream->HasErrorOccurred() == false; 
        }

        AshSize GetExportSize() override
        {
            return sizeof(dataValue);
        }

        T GetDataValue()
        {
            return dataValue;
        }
    private:
        T dataValue;

        static_assert(std::is_fundamental<T>::value == true, "You can only use primitives");
    };

}