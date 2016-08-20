##Background

cppmetrics is a C++ port of the [DropWizard metrics!](https://dropwizard.github.io/metrics/3.1.0/).
The library implements the standard metrics primitives like Gauge, Counter, Histogram, Meter and Timer and the provides the reporter
implementations like the ConsoleReporter, GraphiteRepoter out of the box.
Its written in C++98 to make the integration into existing pre-C++11 codebases easier and should be portable across different 
platforms but being used only in linux environment.

[![Build Status](https://travis-ci.org/ultradns/cppmetrics.png)](https://travis-ci.org/ultradns/cppmetrics)

## Build dependencies
- Pocolib ( >= 1.7.2)
- gtest (>= 1.6.0, dependency for the unit tests only.)

## How to build

make
sudo make install


##Sample code snippet

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

// show how to use the wrapper class
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


###TODO
- Currently the Timer and Meter resolutions are in millis and per-minute respectively, make this configurable.
- Provide more reporters out of the box.


