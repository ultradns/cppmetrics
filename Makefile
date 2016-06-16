#compile  cppmetrics on Mac OS X and linux (ubuntu)
# this makefile is for dev only. The formal compilation is using cmake

GTEST_DIR=$(HOME)/w/googletest
LIBNAME=libcppmetrics_hp.a

# after installing poco in ubutnu, set
# export POCO_BASE=/usr/local/include
UNAME=$(shell uname)

POCO_INC_PATH=     -I $(POCO_BASE)/include  -I $(POCO_BASE)/Net/include -I $(POCO_BASE)/Foundation/include -I $(POCO_BASE)/Util/include
CPPFLAGS=-g -O0 $(POCO_INC_PATH) -I src -I $(GTEST_DIR)/googletest/include  -D_DEBUG -I.. -Wpedantic -pedantic-errors -w  -Wextra  -Wall
CXXFLAGS= -std=c++11 

ifeq ($(UNAME), Darwin)
#	ifndef POCO_BASE
#		$(info POCO_BASE is not defined. I am setting global path based on HomeBrew.)
	    POCO_BASE=/usr/local/Cellar/poco/1.7.2
#	endif
	# for mac:
	LDFLAGS= -L $(POCO_BASE)/lib -L/usr/local/opt/openssl/lib
	CPPFLAGS += -I/usr/local/opt/openssl/include
else
	#for linux(ubuntu 14.04):
#	ifndef POCO_BASE
#		$(info POCO_BASE is not defined.)
		POCO_BASE=/usr/local/include/poco
#	endif
	LDFLAGS= -L /usr/local/lib
endif

LDLIBS=  -lPocoFoundation  -lPocoNet -lPocoUtil 
SRCS=    src/cppmetrics/core/histogram.cpp\
  src/cppmetrics/core/meter.cpp\
  src/cppmetrics/core/metric_registry.cpp\
  src/cppmetrics/core/scheduled_reporter.cpp\
  src/cppmetrics/core/timer_context.cpp\
  src/cppmetrics/core/timer.cpp\
  src/cppmetrics/core/ewma.cpp\
  src/cppmetrics/core/uniform_sample.cpp\
  src/cppmetrics/core/exp_decay_sample.cpp\
  src/cppmetrics/core/snapshot.cpp\
  src/cppmetrics/graphite/graphite_sender_tcp.cpp\
  src/cppmetrics/graphite/graphite_reporter.cpp\
  src/cppmetrics/core/console_reporter.cpp\
  src/cppmetrics/concurrent/simple_scheduled_thread_pool_executor.cpp
	  

OBJS = $(patsubst %.cpp,%.o, $(patsubst %.c,%.o,$(SRCS)))
DEPS= $(OBJS:.o=.d)
-include $(DEPS)

TEST_SRCS= test/cppmetrics/concurrent/test_simple_scheduled_thread_pool_executor.cpp\
			test/cppmetrics/core/test_console_reporter.cpp\
			test/cppmetrics/core/test_counter.cpp\
			test/cppmetrics/core/test_ewma.cpp\
			test/cppmetrics/core/test_exp_decay_sample.cpp\
			test/cppmetrics/core/test_histogram.cpp\
			test/cppmetrics/core/test_meter.cpp\
			test/cppmetrics/core/test_metric_registry.cpp\
			test/cppmetrics/core/test_scheduled_reporter.cpp\
			test/cppmetrics/core/test_snapshot.cpp\
			test/cppmetrics/core/test_timer.cpp\
			test/cppmetrics/core/test_uniform_sample.cpp\
			test/cppmetrics/graphite/test_graphite_reporter.cpp\
			test/cppmetrics/graphite/test_graphite_sender_tcp.cpp
			
			# test/cppmetrics/concurrent/test_simple_thread_pool_executor.cpp
TEST_OBJS = $(patsubst %.cpp,%.o, $(patsubst %.c,%.o,$(TEST_SRCS)))

all: package

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $< -MMD -MF $(@:.o=.d)
	


package: $(OBJS)
	ar -rsv $(LIBNAME) $(OBJS)

clean:
	# I don't know why, but in order to get the wildcard expansion, you need to open bash here
	bash -c 'rm -f src/cppmetrics/*/*.{o,d} $(LIBNAME) test/cppmetrics/*/*.{o,d} test_cppmetrics'
	
main: package
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS) -g -O0 src/testMain.cpp -o sampleApp -L. -lcppmetrics_hp $(LDLIBS)
# ----------------------------------------------------------------------------^^^^^^^^^^^^^^  use the $(LIBNAME) properly here
	
test: $(TEST_OBJS)
	$(CXX)   -g -O0   -lPocoFoundation -o test_cppmetrics -I $(GTEST_DIR)/googletest/include $^ $(LDFLAGS) -L. -lcppmetrics_hp $(LDLIBS)  -L $(GTEST_DIR)/googletest -lgtest -lgtest_main


install:
	./install_the_output.sh
