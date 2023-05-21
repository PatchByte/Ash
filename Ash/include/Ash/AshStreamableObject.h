#pragma once
#include "AshStream.h"

#define ASH_STREAMABLE_OBJECT_IMPLEMENT_SIMPLE(name) const char* GetName() { return name ; } bool Import(AshStream* Stream); bool Export(AshStream* Stream); AshSize GetExportSize();

namespace ash
{

    class AshStreamableObject
    {
    public:
        virtual ~AshStreamableObject() = default;
        virtual const char* GetName() { return "AshStreamableObject(Non implemented)"; }
        virtual bool Import(AshStream* Stream) { return false; }
        virtual bool Export(AshStream* Stream) { return false; }
        virtual AshSize GetExportSize() { return 0; }
    };

}