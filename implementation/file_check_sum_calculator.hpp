#pragma once

#include <string>
#include <filesystem>
#include <unordered_set>

#include <boost/crc.hpp>

enum class CompareStatus
{
   OK,
   FAIL,
   NEW,
   ABSENT
};

struct FileCompareResult
{
   std::string mAbsolutePath;
   CompareStatus mStatus;
   size_t mOldCrc = 0;
   size_t mNewCrc = 0;
};

class SysLogger;

class FileCheckSumCalculator
{
   std::filesystem::path mDirectoryPath;
   std::unordered_map< std::string, size_t > mCheckSums;
   const std::shared_ptr< SysLogger > mSysLogger;
public:
   FileCheckSumCalculator( const std::filesystem::path& directory_path );

   std::vector< FileCompareResult > CalculateAndCompare() const;

private:
   void CheckDeletedFilesAndAddToRes( const std::unordered_set< std::string >& current_files_set,
                                      std::vector< FileCompareResult >& res ) const;
   void CalculateAndStore();
   void SetDirectoryPath( const std::filesystem::path& directory_path );
   size_t Calculate( const std::string& file_path ) const;
   std::size_t FileSize( std::ifstream& file ) const;

};


