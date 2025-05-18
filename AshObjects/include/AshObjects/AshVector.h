#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>
#include <vector>

namespace ash::objects
{

    template<class T>
    class AshVector : public std::vector<T>, public StreamableObject
    {
    public:
        bool Import(Stream* Stream) override
        {
            this->clear();

            bool result = (Stream->HasErrorOccurred() == false);

            Size vectorSize = Stream->Read<Size>();

            for(Size _ = 0; _ < vectorSize; _++)
            {
                T object = T();
                result &= object.Import(Stream);
                this->push_back(object);
            }

            return result;
        }

        bool Export(Stream* Stream) override
        {
            bool result = (Stream->HasErrorOccurred() == false);

            Stream->Write<Size>(this->size());

            for(auto it = this->begin(); it != this->end(); it++)
            {
                result &= it->Export(Stream);
            }

            return result;
        }

        static_assert(std::is_class<T>::value == true && std::is_base_of<StreamableObject, T>::value == true && std::is_pointer<T>::value == false, "Type needs to be a class and needs to base of AshStreamableObject");
    };
}
