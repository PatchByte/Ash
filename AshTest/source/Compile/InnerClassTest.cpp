#include "InnerClassTest.h"

ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT(InnerClassTest)
{
public:
    int var;
};

InnerClassTest::InnerClassTest()
{
    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_ALLOCATION(InnerClassTest);
    classInternalInnerClassTest->var = GetExpectedVar();
}

InnerClassTest::~InnerClassTest()
{
    classInternalInnerClassTest->var = 0;
    ASH_CLASS_IMPLEMENT_INTERNAL_OBJECT_DEALLOCATION(InnerClassTest);
}

int InnerClassTest::GetVar()
{
    return classInternalInnerClassTest->var;
}