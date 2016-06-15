/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * exp_decay_sample.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: vpoliboy
 */


#include <algorithm>
#include "cppmetrics/core/exp_decay_sample.h"
#include "cppmetrics/core/utils.h"

namespace cppmetrics {
namespace core {

const double ExpDecaySample::DEFAULT_ALPHA = 0.015;
const Clock::duration ExpDecaySample::RESCALE_THRESHOLD(
        std::chrono::hours(1));

ExpDecaySample::ExpDecaySample(std::uint32_t reservoir_size, double alpha) :
        alpha_(alpha), reservoir_size_(reservoir_size), count_(0) {
    clear();
    //TODO do we really have to seed ? rng_.seed(get_millis_from_epoch());
}

ExpDecaySample::~ExpDecaySample() {
}

void ExpDecaySample::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    values_.clear();
    count_ = 0;
    start_time_ = Clock::now();
    next_scale_time_ = start_time_ + RESCALE_THRESHOLD;
}

std::uint64_t ExpDecaySample::size() const {
    return std::min(reservoir_size_, count_.load());
}

void ExpDecaySample::update(std::int64_t value) {
    update(value, Clock::now());
}

void ExpDecaySample::update(std::int64_t value,
        const Clock::time_point& timestamp) {
    std::lock_guard<std::mutex> rlock(mutex_);
    rescaleIfNeeded(timestamp);
    std::uniform_real_distribution<> dist(0, 1);
    std::chrono::seconds dur = std::chrono::duration_cast<
            std::chrono::seconds>(timestamp - start_time_);
    double priority = 0.0;
    do {
        priority = std::exp(alpha_ * dur.count()) / dist(rng_);
    } while (std::isnan(priority));

    std::uint64_t count = ++count_;
    if (count <= reservoir_size_) {
        values_[priority] = value;
    } else {
        Double2Int64Map::iterator first_itt(values_.begin());
        double first = first_itt->first;
        if (first < priority
                && values_.insert(std::make_pair(priority, value)).second) {
            values_.erase(first_itt);
        }
    }
}

void ExpDecaySample::rescaleIfNeeded(const Clock::time_point& now) {
    if (next_scale_time_ < now) {
        Clock::time_point prevStartTime = start_time_;
        next_scale_time_ = now + RESCALE_THRESHOLD;
        prevStartTime = start_time_;
        start_time_ = now;
        rescale(prevStartTime);
    }
}

void ExpDecaySample::rescale(const Clock::time_point& prevStartTime) {
    Double2Int64Map old_values;
    std::swap(values_, old_values);
    for(Double2Int64Map::const_iterator it = old_values.begin(); it !=  old_values.end(); ++it) {
        std::chrono::seconds dur = std::chrono::duration_cast<
                std::chrono::seconds>(start_time_ - prevStartTime);
        double key = it->first * std::exp(-alpha_ * dur.count());
        values_[key] = it->second;
    }
    count_ = values_.size();
}

SnapshotPtr ExpDecaySample::getSnapshot() const {
    ValueVector vals;
    vals.reserve(values_.size());
    std::lock_guard<std::mutex> rlock(mutex_);
    for(Double2Int64Map::const_iterator it = values_.begin(); it !=  values_.end(); ++it) {
    	vals.push_back(it->second);
    }
    return SnapshotPtr(new Snapshot(vals));
}

} /* namespace core */
} /* namespace cppmetrics */
