/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * test_simple_scheduled_thread_pool_executor.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: vpoliboy
 */

#include <gtest/gtest.h>
#include "cppmetrics/concurrent/simple_scheduled_thread_pool_executor.h"
#include "cppmetrics/src/cppmetrics/core/reporter.h"
namespace cppmetrics {
namespace concurrent {

namespace {

void timer_handler(std::atomic<size_t>& counter) {
    ++counter;
    usleep(100*1000);
}
}

//TEST(simplescheduledthreadpoolexecutor, fixedDelayTest) {
//    SimpleScheduledThreadPoolExecutor sstpe(3);
//
//    ASSERT_FALSE(sstpe.isShutdown());
//
//    std::atomic<size_t> counter;
//    counter = 0UL;
//    boost::function<void()> timer_task(
//            boost::bind(timer_handler, boost::ref(counter)));
//    sstpe.scheduleAtFixedDelay(timer_task, boost::chrono::milliseconds(100));
//    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
//    // As there is a sleep of 100ms in the timertask, it gets invoked only at every 200ms.
//    ASSERT_LE((size_t )4, counter);
//    ASSERT_GE((size_t )6, counter);
//
//    ASSERT_FALSE(sstpe.isShutdown());
//    sstpe.shutdown();
//    ASSERT_TRUE(sstpe.isShutdown());
//}

static long counter = 0;

class TimerTask : public core::Reporter{
public:
	void report(){
		counter++;
	}
};

TEST(simplescheduledthreadpoolexecutor, fixedRateTest) {
    SimpleScheduledThreadPoolExecutor sstpe(1);

    ASSERT_FALSE(sstpe.isShutdown());
    counter = 0UL;
    TimerTask timer_task;
    sstpe.scheduleAtFixedRate(&timer_task, std::chrono::milliseconds(100));
    usleep(1000*1000);
    ASSERT_LE((size_t )9, counter);
    ASSERT_GE((size_t )10, counter);

    ASSERT_FALSE(sstpe.isShutdown());
    sstpe.shutdown();
    ASSERT_TRUE(sstpe.isShutdown());
}

}
}

