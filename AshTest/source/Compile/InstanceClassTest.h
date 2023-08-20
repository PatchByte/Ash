#pragma once

#include <Ash/AshClass.h>

class InstanceClassTest
{
public:
    InstanceClassTest();

    static constexpr int GetExpectedVar() { return 30; }
    int GetVar();

    ASH_CLASS_IMPLEMENT_SIMPLE_INSTANCE_FUNCTION(InstanceClassTest);
private:
    int var;
};