/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * simple_scheduled_thread_pool_executor.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: vpoliboy
 */

//#include <glog/logging.h>
#include "Poco/Util/TimerTask.h"
#include "Poco/Util/Timer.h"

#include "cppmetrics/concurrent/simple_scheduled_thread_pool_executor.h"
#include "cppmetrics/core/reporter.h"
namespace cppmetrics {
namespace concurrent {


class SchedTaskAdapter :public Poco::Util::TimerTask{
public:
	SchedTaskAdapter(core::Reporter* p) : reporter(p){}
	void run(){
		if(reporter != nullptr)
			reporter->report();
	}
	core::Reporter* reporter;
};

//namespace {
//typedef boost::asio::deadline_timer Timer;
//typedef boost::shared_ptr<Timer> TimerPtr;
//}

//class SimpleScheduledThreadPoolExecutor::TimerTask {
//public:
//    TimerTask() :
//            period_(1000) {
//    };
//    TimerTask(TimerPtr timer, boost::function<void()> task,
//            boost::posix_time::milliseconds period, bool fixed_rate) :
//            timer_(timer), task_(task), period_(period), fixed_rate_(fixed_rate) {
//
//    }
//    TimerPtr timer_;
//    boost::function<void()> task_;
//    boost::posix_time::milliseconds period_;
//    bool fixed_rate_;
//};

SimpleScheduledThreadPoolExecutor::SimpleScheduledThreadPoolExecutor(
        size_t thread_count) :
                running_(true)
                {
   if(thread_count != 1) abort();

}

SimpleScheduledThreadPoolExecutor::~SimpleScheduledThreadPoolExecutor() {
    shutdownNow();
}

void SimpleScheduledThreadPoolExecutor::cancelTimers() {

//    for(Poco::Util::TimerTask& timer_task: timer_tasks_) {
//        timer_task.cancel();
//    }
	if(timer_task_ != nullptr)
		timer_task_->cancel();
}

//void SimpleScheduledThreadPoolExecutor::timerHandler(
//        const boost::system::error_code& ec, size_t timer_index) {
//    if (!running_) {
//        LOG(ERROR)<< "Timer not started.";
//        return;
//    }
//
//    if (ec) {
//        LOG(ERROR) << "Unable to execute the timer, reason " << ec.message();
//        return;
//    }
//
//    TimerTask timer_task;
//    try {
//        boost::lock_guard<boost::mutex> lock(timer_task_mutex_);
//        timer_task = timer_tasks_.at(timer_index);
//    } catch (const std::out_of_range& oor) {
//        LOG(ERROR) << "Unable to find the timer at index " << timer_index;
//        return;
//    }
//
//    if (!timer_task.timer_) {
//        LOG(ERROR) << "Unable to find the timer at index " << timer_index;
//        return;
//    }
//
//    timer_task.task_();
//    boost::system::error_code eec;
//    if (timer_task.fixed_rate_) {
//        timer_task.timer_->expires_at(
//                timer_task.timer_->expires_at() + timer_task.period_, eec);
//    } else {
//        timer_task.timer_->expires_from_now(timer_task.period_, eec);
//    }
//
//    if (eec) {
//        LOG(ERROR) << "Unable to restart the time, reason " << eec.message();
//    }
//
//    timer_task.timer_->async_wait(
//            boost::bind(&SimpleScheduledThreadPoolExecutor::timerHandler, this,
//                    boost::asio::placeholders::error, timer_index));
//}

void SimpleScheduledThreadPoolExecutor::shutdown() {
    if (!running_) {
        return;
    }
    running_ = false;
    timer_.cancel(true);
}

void SimpleScheduledThreadPoolExecutor::shutdownNow() {
    if (!running_) {
        return;
    }
    running_ = false;
    cancelTimers();
//    io_service_.stop();
//    thread_group_.interrupt_all();
//    thread_group_.join_all();
}

bool SimpleScheduledThreadPoolExecutor::isShutdown() const {
    return !running_;
}

//void SimpleScheduledThreadPoolExecutor::scheduleTimer(
//        boost::function<void()> task, boost::chrono::milliseconds interval,
//        bool fixed_rate) {
//    boost::posix_time::milliseconds period(interval.count());
//    TimerPtr timer(new Timer(io_service_, period));
//    size_t timer_index = 0;
//    {
//        boost::lock_guard<boost::mutex> lock(timer_task_mutex_);
//        timer_tasks_.push_back(TimerTask(timer, task, period, fixed_rate));
//        timer_index = timer_tasks_.size() - 1;
//    }
//    timer->async_wait(
//            boost::bind(&SimpleScheduledThreadPoolExecutor::timerHandler, this,
//                    boost::asio::placeholders::error, timer_index));
//}

//void SimpleScheduledThreadPoolExecutor::scheduleAtFixedDelay(
//        boost::function<void()> task, boost::chrono::milliseconds period) {
//    scheduleTimer(task, period, false);
//}

void SimpleScheduledThreadPoolExecutor::scheduleAtFixedRate( core::Reporter* command,
    		std::chrono::milliseconds period){
	timer_task_ = new SchedTaskAdapter(command);
	timer_.scheduleAtFixedRate(timer_task_, 0, period.count());

}


//void SimpleScheduledThreadPoolExecutor::scheduleAtFixedRate(
//        boost::function<void()> task, boost::chrono::milliseconds period) {
//    scheduleTimer(task, period, true);
//}

} /* namespace concurrent */
} /* namespace cppmetrics */
