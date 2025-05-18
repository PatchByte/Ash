#pragma once
#include "Ash/AshResult.h"

class AshBaseRunTest
{
public:
    virtual ash::Result Do() { return ash::Result(false, "Called a non implemented function"); }
};
