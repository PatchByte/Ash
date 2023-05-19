#pragma once
#include <Ash/AshClass.h>

class InnerClassTest 
{
public:
    InnerClassTest();
    ~InnerClassTest();

    int GetVar();
    static constexpr int GetExpectedVar() { return 20; }
private:
    ASH_CLASS_ADD_INTERNAL_OBJECT(InnerClassTest);
};