/*
 * GraphiteSenderUDP.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: noam
 */

#include <sstream>
#include "cppmetrics/graphite/graphite_sender_udp.h"

namespace cppmetrics {
namespace graphite {

GraphiteSenderUDP::GraphiteSenderUDP(const std::string& host,
        uint16_t port) :
                connected_(false),
                host_(host),
                port_(port) {
}

GraphiteSenderUDP::~GraphiteSenderUDP() {
}

void GraphiteSenderUDP::connect() {
	socket_.connect(Poco::Net::SocketAddress(host_,port_));
	connected_ = true;
}

//noamc: current implementation uses blocking send
void GraphiteSenderUDP::send(const std::string& name,
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

void GraphiteSenderUDP::close() {
    connected_ = false;
}

} /* namespace graphite */
} /* namespace cppmetrics */

