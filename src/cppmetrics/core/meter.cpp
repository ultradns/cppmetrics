/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * meter.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: vpoliboy
 */

#include "cppmetrics/core/meter.h"
#include "cppmetrics/core/ewma.h"
#include "cppmetrics/core/types.h"
#include "cppmetrics/core/utils.h"

namespace cppmetrics {
namespace core {

static const uint64_t TICK_INTERVAL =
        Clock::duration(std::chrono::seconds(5)).count();

class Meter::Impl {
public:
    Impl(std::chrono::nanoseconds rate_unit);
    ~Impl();
    uint64_t getCount() const;
    double getFifteenMinuteRate();
    double getFiveMinuteRate();
    double getOneMinuteRate();
    double getMeanRate();
    void mark(uint64_t n);

private:
    const std::chrono::nanoseconds rate_unit_;
    std::atomic<uint64_t> count_;
    const Clock::time_point start_time_;
    std::atomic<uint64_t> last_tick_;
    internal::EWMA m1_rate_;
    internal::EWMA m5_rate_;
    internal::EWMA m15_rate_;

    void tick();
    void tickIfNecessary();
};

Meter::Impl::Impl(std::chrono::nanoseconds rate_unit) :
                rate_unit_(rate_unit),
                count_(0),
                start_time_(Clock::now()),
                last_tick_(
                        std::chrono::duration_cast<std::chrono::nanoseconds>(
                                start_time_.time_since_epoch()).count()),
                m1_rate_(internal::EWMA::oneMinuteEWMA()),
                m5_rate_(internal::EWMA::fiveMinuteEWMA()),
                m15_rate_(internal::EWMA::fifteenMinuteEWMA()) {
}

Meter::Impl::~Impl() {
}

uint64_t Meter::Impl::getCount() const {
    return count_;
}

double Meter::Impl::getFifteenMinuteRate() {
    tickIfNecessary();
    return m15_rate_.getRate();
}

double Meter::Impl::getFiveMinuteRate() {
    tickIfNecessary();
    return m5_rate_.getRate();
}

double Meter::Impl::getOneMinuteRate() {
    tickIfNecessary();
    return m1_rate_.getRate();
}

double Meter::Impl::getMeanRate() {
    uint64_t c = count_;
    if (c > 0) {
        std::chrono::nanoseconds elapsed = std::chrono::duration_cast<
                std::chrono::nanoseconds>(Clock::now() - start_time_);
        return static_cast<double>(c * rate_unit_.count()) / elapsed.count();
    }
    return 0.0;
}

void Meter::Impl::mark(uint64_t n) {
    tickIfNecessary();
    count_ += n;
    m1_rate_.update(n);
    m5_rate_.update(n);
    m15_rate_.update(n);
}

void Meter::Impl::tick() {
    m1_rate_.tick();
    m5_rate_.tick();
    m15_rate_.tick();
}

void Meter::Impl::tickIfNecessary() {
    uint64_t old_tick = last_tick_;
    uint64_t cur_tick =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                    Clock::now().time_since_epoch()).count();
    uint64_t age = cur_tick - old_tick;
    if (age > TICK_INTERVAL) {
        uint64_t new_tick = cur_tick - age % TICK_INTERVAL;
        if (last_tick_.compare_exchange_strong(old_tick, new_tick)) {
            uint64_t required_ticks = age / TICK_INTERVAL;
            for (uint64_t i = 0; i < required_ticks; i++) {
                tick();
            }
        }
    }
}

Meter::Meter(std::chrono::nanoseconds rate_unit) :
        impl_(new Meter::Impl(rate_unit)) {
}

Meter::~Meter() {

}

uint64_t Meter::getCount() const {
    return impl_->getCount();
}

double Meter::getFifteenMinuteRate() {
    return impl_->getFifteenMinuteRate();
}

double Meter::getFiveMinuteRate() {
    return impl_->getFiveMinuteRate();
}

double Meter::getOneMinuteRate() {
    return impl_->getOneMinuteRate();
}

double Meter::getMeanRate() {
    return impl_->getMeanRate();
}

void Meter::mark(uint64_t n) {
    impl_->mark(n);
}

} /* namespace core */
} /* namespace cppmetrics */
