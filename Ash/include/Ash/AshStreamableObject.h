#pragma once
#include "AshStream.h"

#if ASH_OBJECTS_ERASE_NAMES
#define ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE(_) const char* GetName() { return "" ; } bool Import(ash::AshStream* Stream); bool Export(ash::AshStream* Stream); ash::AshSize GetExportSize();
#else
#define ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE(name)    const char* GetName() override { return name ; } \
                                                        bool Import(ash::AshStream* Stream) override;    \
                                                        bool Export(ash::AshStream* Stream) override;    \
                                                        ash::AshSize GetExportSize() override;
#endif


namespace ash
{

    class AshStreamableObject
    {
    public:
        virtual ~AshStreamableObject() = default;

        #if ASH_OBJECTS_ERASE_NAMES
        const char* GetName() { return ""; }
        #else
        virtual const char* GetName() { return "AshStreamableObject(Non implemented)"; }
        #endif

        virtual bool Import(AshStream* Stream) { return false; }
        virtual bool Export(AshStream* Stream) { return false; }
        virtual AshSize GetExportSize() { return 0; }
    };

}