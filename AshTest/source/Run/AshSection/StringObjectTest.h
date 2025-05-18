#pragma once
#include "BaseRunTest.h"

class StringAsciiObjectTest : public AshBaseRunTest
{
public:
    ash::Result Do();
};

class StringWideObjectTest : public AshBaseRunTest
{
public:
    ash::Result Do();
};
