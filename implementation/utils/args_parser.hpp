#pragma once

#include <filesystem>
#include <boost/optional.hpp>
#include <boost/asio.hpp>

// TODO перейти на boost::program_options

struct ProgramArgs
{
   std::filesystem::path mDirectoryPath;
   boost::asio::chrono::seconds mTimerDuration;
};

class ArgsParser
{
public:
   static ProgramArgs GetArgs( int argc, const char *argv[] );
};


