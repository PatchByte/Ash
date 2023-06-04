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

        const char* GetName() 
        { 
            static std::string name = std::string("AshPrimitive").append(typeid(T).name());
            return name.data(); 
        }

        bool Import(AshStream* Stream) 
        {
            dataValue = Stream->Read<T>();
            return Stream->HasErrorOccurred() == false;
        }
        
        bool Export(AshStream* Stream) 
        { 
            Stream->Write<T>(dataValue);
            return Stream->HasErrorOccurred() == false; 
        }

        AshSize GetExportSize() { return sizeof(dataValue); }

        T GetDataValue() { return dataValue; }
    private:
        T dataValue;

        static_assert(std::is_fundamental<T>::value == true, "You can only use primitives");
    };

}