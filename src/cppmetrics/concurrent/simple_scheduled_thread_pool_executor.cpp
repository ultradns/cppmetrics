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
			try {
				reporter->report();
//			} catch(Poco::Net::NetException& ex){
//				fprintf(stderr,"cppmetrics task caught exception: %d", ex.displayText().c_str());
			}catch(std::runtime_error& ex){
				fprintf(stderr,"cppmetrics task caught runtime exception: %d", ex.what());
			}
	}
	core::Reporter* reporter;
};

SimpleScheduledThreadPoolExecutor::SimpleScheduledThreadPoolExecutor(size_t thread_count)
	: running_(true), timer_task_(nullptr){
   if(thread_count != 1) abort();
}

SimpleScheduledThreadPoolExecutor::~SimpleScheduledThreadPoolExecutor() {
    shutdownNow();
}

void SimpleScheduledThreadPoolExecutor::cancelTimers() {
	if(timer_task_ != nullptr)
		timer_task_->cancel();
}

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
}

bool SimpleScheduledThreadPoolExecutor::isShutdown() const {
    return !running_;
}

void SimpleScheduledThreadPoolExecutor::scheduleAtFixedRate( core::Reporter* command,
    		std::chrono::milliseconds period){
	timer_task_ = new SchedTaskAdapter(command);
	timer_.scheduleAtFixedRate(timer_task_, 0, period.count());

}

} /* namespace concurrent */
} /* namespace cppmetrics */
