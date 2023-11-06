#pragma once

#include <memory>

#include <boost/asio.hpp>

class CalculateQueue;

class CalculateTimer
{
   boost::asio::steady_timer mTimer;
   boost::asio::chrono::seconds mPeriod;
   std::shared_ptr< CalculateQueue > mTaskQueue;
public:
   CalculateTimer( const std::shared_ptr< CalculateQueue >& task_queue,
                   const boost::asio::chrono::seconds period, boost::asio::io_context& io_context );

   void Start();

private:
   void Tick();

};
