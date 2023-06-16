#pragma once
#if ASH_ENABLE_LOGGER
#include "BaseRunTest.h"

class AshLoggerTest : public AshBaseRunTest
{
public:
    ash::AshResult Do();
};
#endif