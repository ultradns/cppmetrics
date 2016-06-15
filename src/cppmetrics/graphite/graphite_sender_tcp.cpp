/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * graphite_sender_tcp.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: vpoliboy
 */

#include <sstream>
#include "cppmetrics/graphite/graphite_sender_tcp.h"

namespace cppmetrics {
namespace graphite {

GraphiteSenderTCP::GraphiteSenderTCP(const std::string& host,
        uint16_t port) :
                connected_(false),
                host_(host),
                port_(port) {
}

GraphiteSenderTCP::~GraphiteSenderTCP() {
}

void GraphiteSenderTCP::connect() {

	Poco::Net::SocketAddress addr(host_, port_);
	try{
		socket_.connect(addr);
	}catch(Poco::Exception& ex){
		fprintf(stderr,"GraphiteSenderTCP::connect() ex: %s\n", ex.message().c_str());
		throw std::runtime_error(std::string("Connect() error, reason: ") + ex.message());
	}

	connected_ = true;
}

//noamc: current implementation uses blocking send
void GraphiteSenderTCP::send(const std::string& name,
        const std::string& value,
        uint64_t timestamp) {
    if (!connected_) {
        throw std::runtime_error("Graphite server connection not established.");
    }
    std::ostringstream ostr;
    ostr << name << ' ' << value << ' ' << timestamp << std::endl;
    std::string graphite_str(ostr.str());
    socket_.sendBytes(graphite_str.c_str(), graphite_str.size() );
}

void GraphiteSenderTCP::close() {
    connected_ = false;
    socket_.close();
}

} /* namespace graphite */
} /* namespace cppmetrics */
