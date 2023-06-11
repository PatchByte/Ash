#pragma once
#include "Ash/AshResult.h"

class AshBaseRunTest
{
public:
    virtual ash::AshResult Do() { return ash::AshResult(false, "Called a non implemented function"); }
};