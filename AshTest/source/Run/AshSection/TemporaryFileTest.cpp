#include "TemporaryFileTest.h"
#include <Ash/AshResult.h>
#include <Ash/AshTemporaryFileOrganizer.h>
#include <cstdlib>
#include <string>

ash::AshResult TemporaryFileTest::Do()
{
    ash::AshTemporaryFilePathOrganizer* temporaryFilePathOrganizer = new ash::AshTemporaryFilePathOrganizer("CoolNa1me!");

    temporaryFilePathOrganizer->CreateTemporaryFolder();

    if(temporaryFilePathOrganizer->FindFreePath().GetResult().string() == temporaryFilePathOrganizer->FindFreePath().GetResult().string())
    {
        return ash::AshResult(false, "FindFreePath returned the same result two times.");
    }

    temporaryFilePathOrganizer->RemoveTemporaryFolder();

    delete temporaryFilePathOrganizer;
    temporaryFilePathOrganizer = nullptr;

    return ash::AshResult(true);
}