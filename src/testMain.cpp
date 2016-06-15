/*
 * testMain.cpp
 *
 *  Created on: Jun 14, 2016
 *      Author: noam
 */

#include "scoped_ptr.h"
#include "cppmetrics/cppmetrics.h"
using namespace cppmetrics;

namespace sample {


class GraphiteReporterOptions {
public:
    std::string host_;                  ///<  The graphite server.
    uint32_t port_;              ///<  The graphite port.
    std::string prefix_;                ///<  The prefix to the graphite.
    uint32_t interval_in_secs_;  ///<  The reporting period in secs.
};

/*
 *  Helper class that sets up the default registry and the graphite reporter.
 */
class Controller
{
public:
    cppmetrics::core::MetricRegistryPtr getRegistry() {
        return core::MetricRegistry::DEFAULT_REGISTRY();
    }

    void configureAndStartGraphiteReporter(const GraphiteReporterOptions& graphite_options) {
        if (graphite_reporter_.get() ==  NULL) {
            const std::string& graphite_host(graphite_options.host_);

            uint32_t graphite_port(graphite_options.port_);
            graphite::GraphiteSenderPtr graphite_sender(
                new graphite::GraphiteSenderTCP(graphite_host, graphite_port));

            graphite_reporter_.reset(
                new graphite::GraphiteReporter(getRegistry(), graphite_sender,
                        graphite_options.prefix_));
            graphite_reporter_->start(std::chrono::seconds(graphite_options.interval_in_secs_));
        } else {
            std::cerr << "Graphite reporter already configured.";
        }
    }
private:
    scoped_ptr<cppmetrics::graphite::GraphiteReporter> graphite_reporter_;
};

} //sample

int main(){

	sample::Controller c;
	sample::GraphiteReporterOptions opt;
	opt.port_ = 2003;
	c.configureAndStartGraphiteReporter(opt);


	return 0;
}

