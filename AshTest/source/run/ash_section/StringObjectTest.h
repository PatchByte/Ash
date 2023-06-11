#pragma once
#include "BaseRunTest.h"

class StringAsciiObjectTest : public AshBaseRunTest
{
public:
    ash::AshResult Do();
};

class StringWideObjectTest : public AshBaseRunTest
{
public:
    ash::AshResult Do();
};
