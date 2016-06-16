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

class TrivialGauge : public core::Gauge {
public:
	int64_t value_;
	virtual ~TrivialGauge() {}
    virtual int64_t getValue() {
    	return value_;
    }
};

// measure duration of a code section.
// a histogram is automatically created.
void demo_timer(sample::Controller& c){
	std::default_random_engine rng;
	for (size_t i = 0; i < 100; ++i) {
		std::uniform_real_distribution<> uniform(10, 30);
		size_t sleep_time_milli = 1000*uniform(rng);
		cppmetrics::core::TimerContextPtr time_context(
				c.getRegistry()->timer("test.timer")->timerContextPtr());
		// measure time from here to end of block
		usleep(sleep_time_milli);
	}
}

// use counter to count things -- dah!
void demo_counter(sample::Controller& c){
	cppmetrics::core::CounterPtr numSheeps = c.getRegistry()->counter("sheeps");
	for (size_t i = 0; i < 10; ++i) {
		numSheeps->increment();
		usleep(500*1000);
	}
}

void demo_gauge(sample::Controller& c){
	TrivialGauge fuelLevel;
	core::GaugePtr gauge_ptr(&fuelLevel);
	c.getRegistry()->addGauge("fuel level", gauge_ptr); //registration is done EXACTLY once
	for(int i = 0; i < 100;i++){
		fuelLevel.value_ = i;
		usleep(100*1000);
	}
}

void demo_cppmetrics(){

	sample::Controller c;
	sample::GraphiteReporterOptions opt;
	opt.port_ = 2003;
	opt.host_ = "192.168.99.100"; //"localhost";
	opt.interval_in_secs_ = 1;
	opt.prefix_ = "_";

	// start a thread that will send to Graphite once a second all our counters
	// to see these, " nc -l 2003 "
	c.configureAndStartGraphiteReporter(opt);

	demo_counter(c);
	demo_timer(c);
	demo_gauge(c);

	printf("done!\n");
}

// ------
using namespace std;
typedef cppmetrics::core::CounterPtr Counter;
class CppMetricsWrapper{
	sample::Controller controller;
public:
	void configureAndStartGraphiteReporter(string h, int p, int period, string prefix){
		sample::GraphiteReporterOptions opt;
		opt.port_ = p;
		opt.host_ = h;
		opt.interval_in_secs_ = period;
		opt.prefix_ = prefix;
		controller.configureAndStartGraphiteReporter(opt);
	}

	Counter counter(string name){
		return controller.getRegistry()->counter(name);
	}
};

// show how to use the wrapper class (in house party)
CppMetricsWrapper M;
void demo_cppmetrics2(){

	M.configureAndStartGraphiteReporter("localhost", 2003, 1 , "_");
	Counter numSheeps = M.counter("sheeps");
	for (size_t i = 0; i < 10; ++i) {
		numSheeps->increment();
		usleep(500*1000);
	}
}

int main(){
	printf(" run a graphite server with:\nwhile [ 1 ]; do nc -l 2003;echo ===; done\n");
	//demo_cppmetrics();
	demo_cppmetrics2();
	usleep(3*1000*1000);
	return 0;

}
