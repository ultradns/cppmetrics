/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * simple_thread_pool_executor.h
 *
 *  Created on: Jun 10, 2014
 *      Author: vpoliboy
 */

#ifndef SIMPLE_THREAD_POOL_EXECUTOR_H_
#define SIMPLE_THREAD_POOL_EXECUTOR_H_

#include <atomic>

namespace cppmetrics {
namespace concurrent {

/**
 * A simple threadpool modeled after similar class in java.
 */
class SimpleThreadPoolExecutor {
public:

    /**
     * Creates a new thread pool with the given number of threads.
     * @param thread_count The number of threads in the thread pool.
     */
    SimpleThreadPoolExecutor(size_t thread_count);

    virtual ~SimpleThreadPoolExecutor();

    /**
     * Executes the given task in one of the threads.
     * @param task The task to be executed.
     */
    virtual void execute(std::function<void()> command);

    /**
     * Shuts down the service, may or may not return immediately.
     */
    virtual void shutdown();

    /**
     * Shuts down the service, will return immediately.
     */
    virtual void shutdownNow();

    /**
     * gets the threadpool state.
     * @return True if this is shutdown or shutting down, false otherwise.
     */
    virtual bool isShutdown() const;

private:
    std::atomic<bool> running_;
    //std::asio::io_service io_service_;
    //std::scoped_ptr<std::asio::io_service::work> work_ptr_;
    //std::thread_group thread_group_;
};

} /* namespace concurrent */
} /* namespace cppmetrics */
#endif /* SIMPLE_THREAD_POOL_EXECUTOR_H_ */
