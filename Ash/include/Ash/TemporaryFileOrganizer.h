#ifndef ASH_TEMPORARYFILEORGANIZER_H
#define ASH_TEMPORARYFILEORGANIZER_H

#include <filesystem>
#include <iostream>

#include <Ash/Class.h>
#include <Ash/Result.h>

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

} // namespace ash

#endif // !ASH_TEMPORARYFILEORGANIZER_H
