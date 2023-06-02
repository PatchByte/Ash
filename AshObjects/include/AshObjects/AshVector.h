#pragma once
#include <Ash/AshStreamableObject.h>
#include <iostream>
#include <vector>

namespace ash::objects
{

    /*template<typename T>
    class AshVector : public std::vector<T>, public AshStreamableObject
    {
    public:
        const char* GetName() { return "AshStreamableObject(Non implemented)"; }
        bool Import(AshStream* Stream) { return false; }
        bool Export(AshStream* Stream) { return false; }
        
        AshSize GetExportSize() 
        {

            return 0; 
        }

        static_assert(std::is_class<T>() == true, "Didn't implement classes yet, go fuck yourself.");
    };*/

}