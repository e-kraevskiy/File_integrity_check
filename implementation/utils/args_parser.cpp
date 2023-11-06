#include "args_parser.hpp"

#include <iostream>

#include "sys_logger.hpp"

// TODO перейти на boost::program_options

const int NEEDED_ARGS_COUNT = 2;
const char* FOLDER_ENV_PARAM_NAME = "INTEGRITY_CHECK_DIR";
const char* TIMEOUT_ENV_PARAM_NAME = "INTEGRITY_CHECK_TIMEOUT_SEC";

ProgramArgs ArgsParser::GetArgs( int argc, const char **argv, std::shared_ptr< SysLogger >& sys_logger )
{
   std::filesystem::path dir_path;
   boost::asio::chrono::seconds duration;
   if ( argc == NEEDED_ARGS_COUNT + 1 )
   {
      dir_path = std::filesystem::path( argv[1] );
      duration = boost::asio::chrono::seconds( std::stoi( argv[2] ) );
   } else
   {
      auto dir_path_from_env = std::getenv( FOLDER_ENV_PARAM_NAME );
      auto timeout_from_env = std::getenv( TIMEOUT_ENV_PARAM_NAME );
      if ( dir_path_from_env == nullptr || timeout_from_env == nullptr )
      {
         sys_logger->LogError( "Wrong program arguments" );
         throw std::runtime_error( "ArgsParser::GetArgs. Wrong arguments count." );
      }
      dir_path = std::filesystem::path( dir_path_from_env );
      duration = boost::asio::chrono::seconds( std::stoi( timeout_from_env ) );
   }
   std::cout << "Program argument: DirectoryPath = " << dir_path << std::endl;
   std::cout << "Program argument: TimerDuration in seconds = " << duration.count() << std::endl;
   return ProgramArgs{ dir_path, duration };
}
