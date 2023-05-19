#include "InstanceClassTest.h"

InstanceClassTest::InstanceClassTest():
    var(GetExpectedVar())
{}

int InstanceClassTest::GetVar() { return this->var; }