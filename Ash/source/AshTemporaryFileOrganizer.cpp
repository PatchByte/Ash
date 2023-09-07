#include "Ash/AshBuffer.h"
#include "Ash/AshResult.h"
#include <Ash/AshTemporaryFileOrganizer.h>
#include <Ash/AshCRC32.h>
#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <iterator>
#include <string>

namespace ash
{

    AshTemporaryFilePathOrganizer::AshTemporaryFilePathOrganizer(std::string Name)
    {
        ash::AshBuffer nameBuffer = ash::AshBuffer();
        nameBuffer.CopyPointer((void*)(Name.data()), Name.length());
        ash::AshCRC32Value nameHashValue = ash::AshCRC32Utils::Calculate(0xfeeddeed, &nameBuffer);
        name = "";
        name += ".";
        name += std::to_string(nameHashValue);

        this->CreateTemporaryFolder();
    }

    AshTemporaryFilePathOrganizer::~AshTemporaryFilePathOrganizer()
    {
        this->RemoveTemporaryFolder();
    }

    ash::AshResult AshTemporaryFilePathOrganizer::CreateTemporaryFolder()
    {
        std::filesystem::path temporaryFolderPath = std::filesystem::current_path() / this->name;

        if(std::filesystem::exists(temporaryFolderPath) == true)
        {
            if(auto result = this->RemoveTemporaryFolder(); result.HasError() == true)
            {
                return result;
            }
        }

        std::filesystem::create_directory(temporaryFolderPath);

        return ash::AshResult(true);
    }

    ash::AshResult AshTemporaryFilePathOrganizer::RemoveTemporaryFolder()
    {
        std::filesystem::path temporaryFolderPath = std::filesystem::current_path() / this->name;

        if(std::filesystem::exists(temporaryFolderPath) == false)
        {
            return ash::AshResult(true, "The folder has already been removed.");
        }
    
        for(std::filesystem::directory_entry currentTemporaryFolderPathIteratorEntry : std::filesystem::directory_iterator(temporaryFolderPath))
        {
            if(currentTemporaryFolderPathIteratorEntry.is_directory())
            {
                return ash::AshResult(false, "Found an another directory inside of a temporary directory.");
            }

            if(std::filesystem::remove(currentTemporaryFolderPathIteratorEntry) == false)
            {
                return ash::AshResult(false, "Failed to remove file.");
            }
        }

        std::filesystem::remove(temporaryFolderPath);

        return ash::AshResult(true);
    }

    ash::AshCustomResult<std::filesystem::path> AshTemporaryFilePathOrganizer::FindFreePath()
    {
        std::filesystem::path foundPath = std::filesystem::current_path() / this->name;

        do
        {
            unsigned long long seedModifier = 0;

            ash::AshBuffer seedBufferLeft = ash::AshBuffer();
            ash::AshBuffer seedBufferRight = ash::AshBuffer();

            ash::AshSize temporaryFolderFileCount = std::distance(std::filesystem::directory_iterator(foundPath), std::filesystem::directory_iterator());

            seedBufferLeft.ImportPointer(&this->seed, this->name.length());
            seedBufferRight.ImportPointer(&temporaryFolderFileCount, sizeof(temporaryFolderFileCount));

            seedModifier |= (AshCRC32Utils::Calculate( (AshCRC32Value) (this->seed >> 1), &seedBufferLeft));
            seedModifier <<= 32;
            seedModifier |= (AshCRC32Utils::Calculate( (AshCRC32Value) (this->seed >> 32), &seedBufferRight));
            
            seed = seedModifier;
            seed += 1;

            std::string currentFileNameOfAttempt = std::to_string(seedModifier);

            if(std::filesystem::exists(foundPath / currentFileNameOfAttempt) == false)
            {
                foundPath /= currentFileNameOfAttempt;
                return ash::AshCustomResult<std::filesystem::path>(true).AttachResult(foundPath);
            }

            seed *= 2;
        } while(true);
    }
    
}