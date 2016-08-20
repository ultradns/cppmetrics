/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * utils.h
 *
 *  Created on: Jun 12, 2014
 *      Author: vpoliboy
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>
#include "cppmetrics/core/types.h"

namespace cppmetrics {
namespace core {

//inline time_t get_duration_from_epoch() {
//
//    return std::chrono::system_clock::now().time_since_epoch();
//}

inline uint64_t get_millis_from_epoch() {
    return time(0)*1000;
}

inline uint64_t get_seconds_from_epoch() {
    return time(0);
}
//
//inline std::string utc_timestamp(const std::locale& current_locale) {
//    std::ostringstream ss;
//    // assumes std::cout's locale has been set appropriately for the entire app
//    boost::posix_time::time_facet* t_facet(new boost::posix_time::time_facet());
//    t_facet->time_duration_format("%d-%M-%y %H:%M:%S%F %Q");
//    ss.imbue(std::locale(current_locale, t_facet));
//    ss << boost::posix_time::microsec_clock::universal_time();
//    return ss.str();
//}

}
}

#endif /* UTILS_H_ */
