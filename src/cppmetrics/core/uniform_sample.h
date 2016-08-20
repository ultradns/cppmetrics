/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * uniform_sample.h
 *
 *  Created on: Jun 5, 2014
 *      Author: vpoliboy
 */

#ifndef UNIFORM_SAMPLE_H_
#define UNIFORM_SAMPLE_H_


#include <atomic>
#include <mutex>
#include <random>

#include "cppmetrics/core/sample.h"

namespace cppmetrics {
namespace core {

/**
 * A random sampling reservoir of a stream of {@code long}s. Uses Vitter's Algorithm R to produce a
 * statistically representative sample.
 */
class UniformSample: public Sample {
public:

    /**
     * Creates a new {@link UniformReservoir}.
     * @param size the number of samples to keep in the sampling reservoir
     */
    UniformSample(uint32_t reservoirSize = DEFAULT_SAMPLE_SIZE);
    virtual ~UniformSample();

    /**
     * Clears the values in the sample.
     */
    virtual void clear();

    /**
     * Returns the number of values recorded.
     * @return the number of values recorded
     */
    virtual uint64_t size() const;

    /**
     * Adds a new recorded value to the sample.
     * @param value a new recorded value
     */
    virtual void update(int64_t value);

    /**
     * Returns a snapshot of the sample's values.
     * @return a snapshot of the sample's values
     */
    virtual SnapshotPtr getSnapshot() const;

    /**< The Maximum sample size at any given time. */
    static const uint64_t DEFAULT_SAMPLE_SIZE;
private:
    uint64_t getRandom(uint64_t count) const;
    const uint64_t reservoir_size_;
    std::atomic<uint64_t> count_;
    typedef std::vector<int64_t> Int64Vector;
    Int64Vector values_;
    mutable std::default_random_engine rng_;
    mutable std::mutex mutex_;
};

} /* namespace core */
} /* namespace cppmetrics */
#endif /* UNIFORM_SAMPLE_H_ */
