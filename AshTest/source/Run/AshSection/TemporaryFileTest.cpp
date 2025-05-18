#include "TemporaryFileTest.h"
#include <Ash/AshResult.h>
#include <Ash/AshTemporaryFileOrganizer.h>
#include <cstdlib>
#include <string>

ash::Result TemporaryFileTest::Do()
{
    ash::TemporaryFilePathOrganizer* temporaryFilePathOrganizer = new ash::TemporaryFilePathOrganizer("CoolNa1me!");

    temporaryFilePathOrganizer->CreateTemporaryFolder();

    if(temporaryFilePathOrganizer->FindFreePath().Get().string() == temporaryFilePathOrganizer->FindFreePath().Get().string())
    {
        return ash::Result(false, "FindFreePath returned the same result two times.");
    }

    temporaryFilePathOrganizer->RemoveTemporaryFolder();

    delete temporaryFilePathOrganizer;
    temporaryFilePathOrganizer = nullptr;

    return ash::Result(true);
}
