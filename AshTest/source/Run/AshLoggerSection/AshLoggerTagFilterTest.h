#pragma once
#if ASH_ENABLE_LOGGER
#include "BaseRunTest.h"

class AshLoggerTagFilterTest : public AshBaseRunTest
{
public:
    ash::AshResult Do();
};
#endif