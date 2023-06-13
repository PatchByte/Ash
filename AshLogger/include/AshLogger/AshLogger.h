#pragma once
#include <Ash/AshClass.h>
#include <iostream>

#include "AshLoggerTag.h"
#include "AshLoggerPassage.h"

namespace ash
{

    class AshLogger
    {
    public:
        AshLogger(std::string LoggerName, AshLoggerTagInitializer LoggerTags) {}
        ~AshLogger() {}
    private:
        ASH_CLASS_ADD_INTERNAL_OBJECT(AshLogger);
    };

}