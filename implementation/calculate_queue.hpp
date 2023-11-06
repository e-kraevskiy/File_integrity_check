#pragma once

#include <queue>
#include <memory>
#include <mutex>

class FileCheckSumCalculator;

class CalculateQueue
{
   std::shared_ptr< FileCheckSumCalculator > mCalculator;
   std::queue< bool > mTaskQueue;
   std::mutex mMutex;
public:
   CalculateQueue( const std::shared_ptr< FileCheckSumCalculator > calculator );

   void AddTaskAndStartProcess();

};


