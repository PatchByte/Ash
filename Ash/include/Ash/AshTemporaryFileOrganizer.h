#pragma once
#include <iostream>
#include <filesystem>
#include <Ash/AshClass.h>
#include <Ash/AshResult.h>

namespace ash
{
    class TemporaryFilePathOrganizer;
    class TemporaryFileOrganizerHandle;

    class TemporaryFilePathOrganizer
    {
    public:
        TemporaryFilePathOrganizer(std::string Name);
        TemporaryFilePathOrganizer(TemporaryFilePathOrganizer&) = delete;
        virtual ~TemporaryFilePathOrganizer();
        virtual ash::Result CreateTemporaryFolder();
        virtual ash::Result RemoveTemporaryFolder();
        virtual ash::CustomResult<std::filesystem::path> FindFreePath();

        TemporaryFilePathOrganizer& operator=(TemporaryFilePathOrganizer) = delete;
    private:
        std::string m_Name;
        unsigned long long m_Seed;
    };

}
