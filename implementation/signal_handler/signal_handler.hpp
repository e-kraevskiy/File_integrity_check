#pragma once

#include <memory>

class CalculateQueue;

class SignalHandler
{
   std::shared_ptr< CalculateQueue > mTaskQueue;
public:
   SignalHandler( const std::shared_ptr< CalculateQueue >& task_queue );

   static std::shared_ptr< SignalHandler > Instance();

   static void CreateInstance( const std::shared_ptr< CalculateQueue >& task_queue );

   void HandleUsrSignal();
private:
   static std::shared_ptr< SignalHandler >& InstanceRef();
};