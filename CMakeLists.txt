cmake_minimum_required(VERSION 2.8.7)

project (cppmetrics)

set (CPACK_GENERATOR TGZ)
set (CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set (cppmetrics_VERSION_MAJOR 0)
set (cppmetrics_VERSION_MINOR 1)
set (cppmetrics_VERSION_PATCH 1)
set (cppmetrics_VERSION_TAG "")
set (cppmetrics_VERSION "${cppmetrics_VERSION_MAJOR}.${cppmetrics_VERSION_MINOR}.${cppmetrics_VERSION_PATCH}${cppmetrics_VERSION_TAG}")
set(CPACK_PACKAGE_VERSION ${cppmetrics_VERSION})
INCLUDE(CPack)

set (cppmetrics_PREPROC_FLAGS -DBOOST_NO_CXX11_SMART_PTR)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Werror -Wall -Winit-self -m64 -pipe")

set (BOOST_LD_FLAGS "-lboost_timer -lboost_chrono -lboost_system -lboost_filesystem -lboost_thread -lboost_date_time")
set (GLOG_LD_FLAGS "-lglog")

set (cppmetrics_global_EXTERNAL_HEADERS
  src/cppmetrics/cppmetrics.h
)

set (cppmetrics_SOURCES
  src/cppmetrics/core/histogram.cpp
  src/cppmetrics/core/meter.cpp
  src/cppmetrics/core/metric_registry.cpp
  src/cppmetrics/core/scheduled_reporter.cpp
  src/cppmetrics/core/timer_context.cpp
  src/cppmetrics/core/timer.cpp
  src/cppmetrics/concurrent/simple_thread_pool_executor.cpp
  src/cppmetrics/concurrent/simple_scheduled_thread_pool_executor.cpp
  src/cppmetrics/core/ewma.cpp
  src/cppmetrics/core/exp_decay_sample.cpp
  src/cppmetrics/core/snapshot.cpp
  src/cppmetrics/core/uniform_sample.cpp
  src/cppmetrics/graphite/graphite_sender_tcp.cpp
  src/cppmetrics/graphite/graphite_reporter.cpp
  src/cppmetrics/core/console_reporter.cpp
)

set (cppmetrics_core_EXTERNAL_HEADERS
  src/cppmetrics/core/counter.h
  src/cppmetrics/core/histogram.h
  src/cppmetrics/core/gauge.h
  src/cppmetrics/core/meter.h
  src/cppmetrics/core/metered.h
  src/cppmetrics/core/metric.h
  src/cppmetrics/core/metric_registry.h
  src/cppmetrics/core/reporter.h
  src/cppmetrics/core/scheduled_reporter.h
  src/cppmetrics/core/timer.h
  src/cppmetrics/core/timer_context.h
  src/cppmetrics/core/exp_decay_sample.h
  src/cppmetrics/core/sample.h
  src/cppmetrics/core/snapshot.h
  src/cppmetrics/core/uniform_sample.h
  src/cppmetrics/core/sampling.h
  src/cppmetrics/core/types.h
  src/cppmetrics/core/console_reporter.h
 )

set (cppmetrics_graphite_EXTERNAL_HEADERS  
  src/cppmetrics/graphite/graphite_sender.h
  src/cppmetrics/graphite/graphite_sender_tcp.h
  src/cppmetrics/graphite/graphite_reporter.h
)

set (cppmetrics_concurrent_EXTERNAL_HEADERS  
  src/cppmetrics/concurrent/simple_thread_pool_executor.h
  src/cppmetrics/concurrent/simple_scheduled_thread_pool_executor.h
)

set (cppmetrics_HEADERS
  src/cppmetrics/core/ewma.h
  ${cppmetrics_core_EXTERNAL_HEADERS}
  ${cppmetrics_concurrent_EXTERNAL_HEADERS}
  ${cppmetrics_graphite_EXTERNAL_HEADERS}
)

set (cppmetrics_core_TEST_SOURCES
  test/cppmetrics/core/test_counter.cpp
  test/cppmetrics/core/test_ewma.cpp
  test/cppmetrics/core/test_exp_decay_sample.cpp
  test/cppmetrics/core/test_histogram.cpp
  test/cppmetrics/core/test_meter.cpp
  test/cppmetrics/core/test_metric_registry.cpp
  test/cppmetrics/core/test_scheduled_reporter.cpp
  test/cppmetrics/core/test_snapshot.cpp
  test/cppmetrics/core/test_timer.cpp
  test/cppmetrics/core/test_uniform_sample.cpp
  test/cppmetrics/core/test_console_reporter.cpp
)

set (cppmetrics_graphite_TEST_SOURCES
  test/cppmetrics/graphite/test_graphite_sender_tcp.cpp
  test/cppmetrics/graphite/test_graphite_reporter.cpp
)

set (cppmetrics_concurrent_TEST_SOURCES
  test/cppmetrics/concurrent/test_simple_thread_pool_executor.cpp
  test/cppmetrics/concurrent/test_simple_scheduled_thread_pool_executor.cpp
)

set (cppmetrics_TEST_SOURCES
    ${cppmetrics_core_TEST_SOURCES}
    ${cppmetrics_concurrent_TEST_SOURCES}
    ${cppmetrics_graphite_TEST_SOURCES}
)

# Setup the vendor dist library.
IF (UNIX)
    IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=auto -D__APPLE__ -stdlib=libstdc++")
        IF (NOT DEFINED VENDOR_DIST_DIR)
            set (VENDOR_DIST_DIR ${CMAKE_SOURCE_DIR}/vendor_dist_mac/dist/Darwin_x86_64/)
        ENDIF ()
      ELSE ()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=k8")   
        set (BOOST_LD_FLAGS "${BOOST_LD_FLAGS} -lpthread -lrt")
        IF (NOT DEFINED VENDOR_DIST_DIR)
            set (VENDOR_TAG trunk_85459)
            set (VENDOR_DIST_ENV Linux_x86_64)
            set (VENDOR_DIST vendor-dist)
            set (VENDOR_DIST_DIR ${CMAKE_CURRENT_BINARY_DIR}/vendor-dist/dist/Linux_x86_64/)
            add_custom_command(OUTPUT ${VENDOR_DIST}
                COMMAND mkdir ${VENDOR_DIST}
                # CMAKE has problems with space delimited strings.
                COMMAND svn export --username $ENV{SVN_USERNAME} --password $ENV{SVN_PASSWORD} --no-auth-cache https://svn.ultradns.net/svn/vendor/tags/${VENDOR_TAG}/dist/${VENDOR_DIST_ENV}.tgz ${VENDOR_DIST}/${VENDOR_DIST_ENV}.tgz
                COMMAND cd ${VENDOR_DIST} && tar xf ${VENDOR_DIST_ENV}.tgz
            )
        ENDIF ()
    ENDIF ()
ENDIF ()

set (GTEST_LD_FLAGS "${BOOST_LD_FLAGS} ${GLOG_LD_FLAGS} -lgtest_main -lgtest")

add_custom_target(vendor_dist DEPENDS ${VENDOR_DIST})


set (DIST_INCLUDE_DIR include/cppmetrics)

# HACK: Remove any old versions of cppmetrics that are built as part of vendor-libs.            
# cppmetrics is also built as part of the vendor-dist we need to clean up that before we proceed.
# kind of chicken-egg problem.
add_custom_target(clean_cppmetrics_from_vendor_dist 
    COMMAND rm -rf ${VENDOR_DIST_DIR}/${DIST_INCLUDE_DIR}
    COMMAND find ${VENDOR_DIST_DIR}/lib -name \"*cppmetrics*\" -exec rm -rf {} "\;"
)

include_directories (${VENDOR_DIST_DIR}/include src/) 
link_directories (${VENDOR_DIST_DIR}/lib) 


install (FILES ${cppmetrics_global_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR})
install (FILES ${cppmetrics_core_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR}/core)
install (FILES ${cppmetrics_concurrent_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR}/concurrent)
install (FILES ${cppmetrics_graphite_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR}/graphite)

add_library (cppmetrics STATIC ${cppmetrics_HEADERS} ${cppmetrics_SOURCES})
install (TARGETS cppmetrics DESTINATION lib)

# Re-enable them when the boost is built with shared library option too.
# add_library (cppmetricsshared SHARED ${cppmetrics_HEADERS} ${cppmetrics_SOURCES})
# target_link_libraries (cppmetricsshared ${BOOST_LD_FLAGS})
# install (TARGETS cppmetricsshared DESTINATION lib)

add_executable(gtest_cppmetrics EXCLUDE_FROM_ALL ${cppmetrics_TEST_SOURCES})
target_link_libraries (gtest_cppmetrics cppmetrics ${GTEST_LD_FLAGS})
add_custom_target(gtest gtest_cppmetrics --gtest_output=xml DEPENDS gtest_cppmetrics)

