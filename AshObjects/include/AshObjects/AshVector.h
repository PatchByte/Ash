#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>
#include <vector>

namespace ash::objects
{

    template<class T>
    class AshVector : public std::vector<T>, public AshStreamableObject
    {
    public:
        bool Import(AshStream* Stream) override
        {
            this->clear();

            bool result = (Stream->HasErrorOccurred() == false);

            AshSize vectorSize = Stream->Read<AshSize>();

            for(AshSize _ = 0; _ < vectorSize; _++)
            {
                T object = T();
                result &= object.Import(Stream);
                this->push_back(object);
            }

            return result; 
        }

        bool Export(AshStream* Stream) override
        {
            bool result = (Stream->HasErrorOccurred() == false);

            Stream->Write<AshSize>(this->size());
            
            for(auto it = this->begin(); it != this->end(); it++)
            {
                result &= it->Export(Stream);
            }

            return result;
        }
        
        static_assert(std::is_class<T>::value == true && std::is_base_of<AshStreamableObject, T>::value == true && std::is_pointer<T>::value == false, "Type needs to be a class and needs to base of AshStreamableObject");
    };
}
