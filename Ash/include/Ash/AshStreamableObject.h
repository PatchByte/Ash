#pragma once
#include "AshStream.h"

#if ASH_OBJECTS_ERASE_NAMES
#define ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE(_) bool Import(ash::Stream* Stream); \
                                                  bool Export(ash::Stream* Stream);
#else
#define ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE(name)    const char* GetName() override { return name ; } \
                                                        bool Import(ash::Stream* Stream) override;    \
                                                        bool Export(ash::Stream* Stream) override;
#endif


namespace ash
{

    class StreamableObject
    {
    public:
        virtual ~StreamableObject() = default;

        #if ASH_OBJECTS_ERASE_NAMES
        inline const char* GetName() { return ""; }
        #else
        virtual const char* GetName() { return "AshStreamableObject"; }
        #endif

        virtual bool Import(Stream* Stream) { return false; }
        virtual bool Export(Stream* Stream) { return false; }
    };

}
