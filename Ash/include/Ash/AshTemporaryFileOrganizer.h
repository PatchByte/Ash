#pragma once
#include <iostream>
#include <filesystem>
#include <Ash/AshClass.h>
#include <Ash/AshResult.h>

namespace ash
{
    class AshTemporaryFilePathOrganizer;
    class AshTemporaryFileOrganizerHandle;

    class AshTemporaryFilePathOrganizer
    {
    public:
        AshTemporaryFilePathOrganizer(std::string Name);
        AshTemporaryFilePathOrganizer(AshTemporaryFilePathOrganizer&) = delete;
        virtual ~AshTemporaryFilePathOrganizer();
        virtual ash::AshResult CreateTemporaryFolder();
        virtual ash::AshResult RemoveTemporaryFolder();
        virtual ash::AshCustomResult<std::filesystem::path> FindFreePath();

        AshTemporaryFilePathOrganizer& operator=(AshTemporaryFilePathOrganizer) = delete;
    private:
        std::string name;
        unsigned long long seed;
    };

}