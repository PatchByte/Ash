#pragma once
#include "AshStream.h"

#if ASH_OBJECTS_ERASE_NAMES
#define ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE(_) bool Import(ash::AshStream* Stream); \
                                                  bool Export(ash::AshStream* Stream);
#else
#define ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE(name)    const char* GetName() override { return name ; } \
                                                        bool Import(ash::AshStream* Stream) override;    \
                                                        bool Export(ash::AshStream* Stream) override;
#endif


namespace ash
{

    class AshStreamableObject
    {
    public:
        virtual ~AshStreamableObject() = default;

        #if ASH_OBJECTS_ERASE_NAMES
        inline const char* GetName() { return ""; }
        #else
        virtual const char* GetName() { return "AshStreamableObject"; }
        #endif

        virtual bool Import(AshStream* Stream) { return false; }
        virtual bool Export(AshStream* Stream) { return false; }
    };

}
