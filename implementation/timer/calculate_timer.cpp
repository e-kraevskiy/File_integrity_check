#include "calculate_timer.hpp"

#include <iostream>
#include <boost/bind.hpp>

#include "../calculate_queue.hpp"


CalculateTimer::CalculateTimer( const std::shared_ptr< CalculateQueue >& task_queue,
                                const boost::asio::chrono::seconds period,
                                boost::asio::io_context& io_context )
   :mTimer( io_context, period ),
   mPeriod( period ),
   mTaskQueue( task_queue ){}

void CalculateTimer::Start()
{
   mTimer.async_wait( boost::bind( &CalculateTimer::Tick, this ) );
}

void CalculateTimer::Tick()
{
   std::cout << "CalculateTimer::Tick" << std::endl;
   mTaskQueue->AddTaskAndStartProcess();
   mTimer.expires_at( mTimer.expiry() + mPeriod );
   mTimer.async_wait( boost::bind( &CalculateTimer::Tick, this ) );
}


