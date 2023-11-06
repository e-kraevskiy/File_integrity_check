#include <iostream>
#include <string>
#include <fstream>

#include "file_check_sum_calculator.hpp"
#include "utils/args_parser.hpp"
#include "utils/sys_logger.hpp"
#include "calculate_queue.hpp"
#include "timer/calculate_timer.hpp"
#include "signal_handler/signal_handler.hpp"


int main( int argc, const char** argv )
{
   try
   {
      const auto program_args = ArgsParser::GetArgs( argc, argv );
      auto calculator = std::make_shared< FileCheckSumCalculator >( program_args.mDirectoryPath );
      auto queue = std::make_shared< CalculateQueue >( calculator );
      boost::asio::io_context io_context;
      auto timer = std::make_shared< CalculateTimer >( queue, program_args.mTimerDuration, io_context );
      SignalHandler::CreateInstance( queue );
      timer->Start();
      io_context.run();
      return 0;
   } catch( std::exception& exc )
   {
      std::cout << "Program failed. Exception = " << exc.what() << std::endl;
      return -1;
   }
}