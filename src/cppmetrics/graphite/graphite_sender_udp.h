/*
 * GraphiteSenderUDP.h
 *
 *  Created on: Jul 27, 2016
 *      Author: noam
 */

#ifndef SRC_CPPMETRICS_GRAPHITE_GRAPHITE_SENDER_UDP_H_
#define SRC_CPPMETRICS_GRAPHITE_GRAPHITE_SENDER_UDP_H_

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/DatagramSocket.h>
#include "cppmetrics/graphite/graphite_sender.h"

namespace cppmetrics {
namespace graphite {

class GraphiteSenderUDP: public GraphiteSender {
	public:
	    /**
	     * Creates a new sender with the given params.
	     * @param host The graphite server host.
	     * @param port The graphite server port.
	     */
		GraphiteSenderUDP(const std::string& host, uint16_t port);
	    virtual ~GraphiteSenderUDP();

	    /**
	     * Connects to the graphite sender over UDP.
	     * does nothing.
	     * @throws nothing
	     */
	    virtual void connect();

	    /**
	     * Posts the metric name, value and timestamp to the graphite server.
	     * @param name The name of the metric
	     * @param value The value of the metric
	     * @param timestamp The timestamp of the metric.

	     * @throws std::runtime_error if there is a problem.
	     */
	    virtual void send(const std::string& name,
	            const std::string& value,
	            uint64_t timestamp);

	    /**
	     * Closes the connection.
	     * nothing to do for UDP
	     */
	    virtual void close();

	private:
	    bool connected_;
	    std::string host_;
	    uint16_t port_;

	   Poco::Net::DatagramSocket	socket_;
};

}} // namespace

#endif /* SRC_CPPMETRICS_GRAPHITE_GRAPHITE_SENDER_UDP_H_ */
