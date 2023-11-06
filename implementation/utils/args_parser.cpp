#include "args_parser.hpp"

#include <iostream>

int NEEDED_ARGS_COUNT = 2;

ProgramArgs ArgsParser::GetArgs( int argc, const char **argv )
{
   if ( argc != NEEDED_ARGS_COUNT + 1 )
      throw std::runtime_error( "ArgsParser::GetArgs. Wrong arguments count." );
   auto dir_path = std::filesystem::path( argv[1] );
   auto duration = boost::asio::chrono::seconds( std::stoi( argv[2] ) );
   std::cout << "Program argument 1. DirectoryPath = " << dir_path << std::endl;
   std::cout << "Program argument 2. TimerDuration in seconds = " << duration.count() << std::endl;
   return ProgramArgs{ dir_path, duration };
}
