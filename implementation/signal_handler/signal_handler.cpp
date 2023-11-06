#include "signal_handler.hpp"
#include <csignal>
#include <assert.h>
#include <iostream>

#include "../calculate_queue.hpp"

SignalHandler::SignalHandler( const std::shared_ptr< CalculateQueue >& task_queue )
   :mTaskQueue( task_queue )
{
   // Signals to ignore:
   std::signal( SIGQUIT, SIG_IGN);
   std::signal( SIGINT, SIG_IGN);
   std::signal( SIGHUP, SIG_IGN);
   std::signal( SIGSTOP, SIG_IGN);
   std::signal( SIGCONT, SIG_IGN);
   // Signals to process:
   std::signal( SIGUSR1, []( int )
   {
      SignalHandler::Instance()->HandleUsrSignal();
   });

}

std::shared_ptr< SignalHandler >& SignalHandler::InstanceRef()
{
   static std::shared_ptr< SignalHandler > INSTANCE;
   return INSTANCE;
}

void SignalHandler::HandleUsrSignal()
{
   std::cout << "SignalHandler::HandleUsrSignal" << std::endl;
   mTaskQueue->AddTaskAndStartProcess();
}

std::shared_ptr< SignalHandler > SignalHandler::Instance()
{
   assert( InstanceRef() );
   return InstanceRef();
}

void SignalHandler::CreateInstance( const std::shared_ptr< CalculateQueue >& task_queue )
{
   InstanceRef() = std::make_shared< SignalHandler >( task_queue );
}
