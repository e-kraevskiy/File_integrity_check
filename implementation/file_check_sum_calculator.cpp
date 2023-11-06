#include "file_check_sum_calculator.hpp"

#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_set>

#include <boost/crc.hpp>

#include "utils/utils.hpp"
#include "utils/sys_logger.hpp"

FileCheckSumCalculator::FileCheckSumCalculator( const std::filesystem::path& directory_path,
                                                const std::shared_ptr< SysLogger >& sys_logger )
  : mSysLogger( sys_logger )
{
   SetDirectoryPath( directory_path );
   CalculateAndStore();
}

size_t FileCheckSumCalculator::Calculate( const std::string& file_path ) const
{
   std::ifstream ifstream{ file_path, std::ios::in | std::ios::binary };
   const std::size_t file_size = FileSize( ifstream );

   std::vector< char > buf( file_size );
   ifstream.read( &buf[0], file_size );

   boost::crc_32_type result;
   result.process_bytes( &buf[0], file_size );

   return result.checksum();
}

std::size_t FileCheckSumCalculator::FileSize( std::ifstream& file ) const
{
   std::streampos current = file.tellg();
   file.seekg( 0, std::ios::end );
   std::size_t result = file.tellg();
   file.seekg( current, std::ios::beg );
   return result;
}

void FileCheckSumCalculator::SetDirectoryPath( const std::filesystem::path& directory_path )
{
   if ( !mDirectoryPath.empty() )
      throw std::runtime_error( "FileCheckSumCalculator::SetDirectoryPath. Directory path already set" );
   if ( !exists( directory_path ) )
      throw std::filesystem::filesystem_error( "FileCheckSumCalculator::SetDirectoryPath. Directory path not valid.", std::io_errc() );
   mDirectoryPath = directory_path;
}

void FileCheckSumCalculator::CalculateAndStore()
{
   if ( !mCheckSums.empty() )
      throw std::runtime_error( "FileCheckSumCalculator::CalculateAndStore. Check sums was already calculated." );
   for ( const auto& directory_entry : std::filesystem::directory_iterator( mDirectoryPath ) )
   {
      auto file_path = directory_entry.path();
      auto check_sum = Calculate( file_path );
      std::cout << "Check sum for file " << absolute( file_path ) << " = " << check_sum << std::endl;
      mCheckSums.insert( std::make_pair( file_path.filename(), check_sum ) );
   }
}

std::vector< FileCompareResult > FileCheckSumCalculator::CalculateAndCompare() const
{
   if ( mCheckSums.empty() )
   {
      std::cout << "FileCheckSumCalculator::CalculateAndCompare. Check sums empty, nothing to compare" ;
      return {};
   }
   std::unordered_set< std::string > current_files_set;
   std::vector< FileCompareResult > res;
   for ( const auto& directory_entry : std::filesystem::directory_iterator( mDirectoryPath ) )
   {
      const auto& absolute_path = directory_entry.path();
      auto new_crc = Calculate( absolute_path );
      auto file_name = absolute_path.filename();
      current_files_set.insert( file_name );

      FileCompareResult compare_result;
      compare_result.mAbsolutePath = absolute_path;
      if ( mCheckSums.contains( file_name ) )
      {
         auto old_crc = mCheckSums.at( file_name );
         compare_result.mOldCrc = old_crc;
         compare_result.mNewCrc = new_crc;
         if ( old_crc != new_crc )
            compare_result.mStatus = CompareStatus::FAIL;
         else
            compare_result.mStatus = CompareStatus::OK;
         std::cout << "FileCheckSumCalculator::CalculateAndCompare. Comparing file = " << compare_result.mAbsolutePath
                   << ". Status = " << CompareStatusToString( compare_result.mStatus ) << std::endl;
         res.push_back( compare_result );
      } else
      {
         compare_result.mStatus = CompareStatus::NEW;
         std::cout << "FileCheckSumCalculator::CalculateAndCompare. Found new file: " << compare_result.mAbsolutePath << std::endl;
         res.push_back( compare_result );
      }
      mSysLogger->Log( compare_result );
   }

   CheckDeletedFilesAndAddToRes( current_files_set, res );

   return res;
}

void FileCheckSumCalculator::CheckDeletedFilesAndAddToRes( const std::unordered_set< std::string >& current_files_set,
                                                           std::vector< FileCompareResult >& res ) const
{
   for ( const auto& file_to_check_sum: mCheckSums )
   {
      if ( !current_files_set.contains( file_to_check_sum.first ) )
      {
         FileCompareResult compare_result;
         compare_result.mStatus = CompareStatus::ABSENT;
         compare_result.mAbsolutePath = mDirectoryPath.string() + file_to_check_sum.first;
         mSysLogger->Log( compare_result );
         res.push_back( compare_result );
      }
   }
}


