#include "Ash/Buffer.hpp"
#include "Ash/Result.hpp"
#include "Ash/CRC32.hpp"
#include "Ash/TemporaryFileOrganizer.hpp"

#include <filesystem>
#include <iterator>
#include <string>

namespace ash
{

    TemporaryFilePathOrganizer::TemporaryFilePathOrganizer(std::string Name)
    {
        ash::Buffer nameBuffer = ash::Buffer();
        nameBuffer.CopyPointer((void*)(Name.data()), Name.length());
        ash::CRC32 nameHashValue = ash::CRC32Utils::Calculate(0xfeeddeed, &nameBuffer);
        m_Name = "";
        m_Name += ".";
        m_Name += std::to_string(nameHashValue);

        this->CreateTemporaryFolder();
    }

    TemporaryFilePathOrganizer::~TemporaryFilePathOrganizer()
    {
        this->RemoveTemporaryFolder();
    }

    ash::Result TemporaryFilePathOrganizer::CreateTemporaryFolder()
    {
        std::filesystem::path temporaryFolderPath = std::filesystem::current_path() / this->m_Name;

        if (std::filesystem::exists(temporaryFolderPath) == true)
        {
            if (auto result = this->RemoveTemporaryFolder(); result.HasError() == true)
            {
                return result;
            }
        }

        std::filesystem::create_directory(temporaryFolderPath);

        return ash::Result(true);
    }

    ash::Result TemporaryFilePathOrganizer::RemoveTemporaryFolder()
    {
        std::filesystem::path temporaryFolderPath = std::filesystem::current_path() / this->m_Name;

        if (std::filesystem::exists(temporaryFolderPath) == false)
        {
            return ash::Result(true, "The folder has already been removed.");
        }

        for (std::filesystem::directory_entry currentTemporaryFolderPathIteratorEntry : std::filesystem::directory_iterator(temporaryFolderPath))
        {
            if (currentTemporaryFolderPathIteratorEntry.is_directory())
            {
                return ash::Result(false, "Found an another directory inside of a temporary directory.");
            }

            if (std::filesystem::remove(currentTemporaryFolderPathIteratorEntry) == false)
            {
                return ash::Result(false, "Failed to remove file.");
            }
        }

        std::filesystem::remove(temporaryFolderPath);

        return ash::Result(true);
    }

    ash::CustomResult<std::filesystem::path> TemporaryFilePathOrganizer::FindFreePath()
    {
        std::filesystem::path foundPath = std::filesystem::current_path() / this->m_Name;

        do
        {
            unsigned long long seedModifier = 0;

            ash::Buffer seedBufferLeft = ash::Buffer();
            ash::Buffer seedBufferRight = ash::Buffer();

            ash::Size temporaryFolderFileCount = std::distance(std::filesystem::directory_iterator(foundPath), std::filesystem::directory_iterator());

            seedBufferLeft.ImportPointer(&this->m_Name, this->m_Name.length());
            seedBufferRight.ImportPointer(&temporaryFolderFileCount, sizeof(temporaryFolderFileCount));

            seedModifier |= (CRC32Utils::Calculate((CRC32)(this->m_Seed >> 1), &seedBufferLeft));
            seedModifier <<= 32;
            seedModifier |= (CRC32Utils::Calculate((CRC32)(this->m_Seed >> 32), &seedBufferRight));

            m_Seed = seedModifier;
            m_Seed += 1;

            std::string currentFileNameOfAttempt = std::to_string(seedModifier);

            if (std::filesystem::exists(foundPath / currentFileNameOfAttempt) == false)
            {
                foundPath /= currentFileNameOfAttempt;
                return ash::CustomResult<std::filesystem::path>(true).Attach(foundPath);
            }

            m_Seed *= 2;
        } while (true);
    }

} // namespace ash
