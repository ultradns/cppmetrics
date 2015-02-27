#!/bin/bash

GLOG_TARGET=glog-0.3.3
GLOG_TARGET_PACKAGE=${GLOG_TARGET}.tar.gz
GTEST_TARGET=gtest-1.7.0
GTEST_TARGET_PACKAGE=${GTEST_TARGET}.zip

make_glog() {
    INSTALL_DIR=$1
    echo "Fetching glog package."
    curl https://google-glog.googlecode.com/files/${GLOG_TARGET_PACKAGE} -o ${GLOG_TARGET_PACKAGE}
    echo "Unzipping ${GLOG_TARGET_PACKAGE} package."
    tar -zxvf ${GLOG_TARGET_PACKAGE}
    cd ${GLOG_TARGET} && ./configure --prefix=${INSTALL_DIR} --enable-shared=false
    make install
}        


make_gtest() {
    INSTALL_DIR=$1
    curl -v https://googletest.googlecode.com/files/${GTEST_TARGET_PACKAGE} -o ${GTEST_TARGET_PACKAGE}
    unzip ${GTEST_TARGET_PACKAGE}
    cd ${GTEST_TARGET} && ./configure --prefix=${INSTALL_DIR} --enable-shared=false
    make
    echo "Installing from ${PWD} into ${INSTALL_DIR}"
    test -z "${INSTALL_DIR}/lib" || /bin/mkdir -p "${INSTALL_DIR}/lib"
    # since libtool doesnt have same params in all versions..
    /usr/bin/install -c ./lib/.libs/libgtest.lai ${INSTALL_DIR}/lib/libgtest.la
    /usr/bin/install -c ./lib/.libs/libgtest.a ${INSTALL_DIR}/lib/libgtest.a
    /usr/bin/install -c ./lib/.libs/libgtest_main.lai ${INSTALL_DIR}/lib/libgtest_main.la
    /usr/bin/install -c ./lib/.libs/libgtest_main.a ${INSTALL_DIR}/lib/libgtest_main.a
    chmod 644 ${INSTALL_DIR}/lib/libgtest*.a
    ranlib ${INSTALL_DIR}/lib/libgtest*.a
    test -z "${INSTALL_DIR}/include/gtest" || /bin/mkdir -p "${INSTALL_DIR}/include/gtest"
    test -z "${INSTALL_DIR}/include/gtest/internal" || /bin/mkdir -p "${INSTALL_DIR}/include/gtest/internal"
    find include -name "*.h"  -exec /usr/bin/install -c -m 644 '{}' "${INSTALL_DIR}/{}" \;
}

fetch_and_install_dependencies() {
    DEPENDENCY_DIR=$1
    rm -rf ${DEPENDENCY_DIR}/*
    SOURCE_DIR=${DEPENDENCY_DIR}/sources
    INSTALL_DIR=${DEPENDENCY_DIR}/install
    mkdir -p ${SOURCE_DIR}
    echo "Created ${SOURCE_DIR}"
    mkdir -p ${INSTALL_DIR}
    echo "Created ${INSTALL_DIR}"
    cd ${SOURCE_DIR} && make_glog ${INSTALL_DIR}
    cd ${SOURCE_DIR} && make_gtest ${INSTALL_DIR}
}

fetch_and_install_dependencies $1
