#include "calculate_queue.hpp"

#include "file_check_sum_calculator.hpp"

CalculateQueue::CalculateQueue( const std::shared_ptr< FileCheckSumCalculator > calculator )
  :mCalculator( calculator ) {};

void CalculateQueue::AddTaskAndStartProcess()
{
   std::lock_guard< std::mutex> lock( mMutex );
   mTaskQueue.push( true );

   while ( !mTaskQueue.empty() )
   {
      mCalculator->CalculateAndCompare();
      mTaskQueue.pop();
   }
}
