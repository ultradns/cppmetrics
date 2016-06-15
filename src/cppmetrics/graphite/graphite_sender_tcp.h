/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * graphite_sender_tcp.h
 *
 *  Created on: Jun 16, 2014
 *      Author: vpoliboy
 */

#ifndef GRAPHITE_SENDER_TCP_H_
#define GRAPHITE_SENDER_TCP_H_

#include "Poco/Net/StreamSocket.h"
#include "cppmetrics/graphite/graphite_sender.h"

namespace cppmetrics {
namespace graphite {

/**
 * Graphite TCP sender.
 */
class GraphiteSenderTCP: public GraphiteSender {
public:

    /**
     * Creates a new sender with the given params.
     * @param host The graphite server host.
     * @param port The graphite server port.
     */
    GraphiteSenderTCP(const std::string& host, uint16_t port);
    virtual ~GraphiteSenderTCP();

    /**
     * Connects to the graphite sender over TCP.
     * @return True on success, false otherwise.
     * @throws std::runtime_error if there is a problem.
     */
    virtual void connect();

    /**
     * Posts the metric name, value and timestamp to the graphite server.
     * @param name The name of the metric
     * @param value The value of the metric
     * @param timestamp The timestamp of the metric.
     * @return True on success false otherwise.
     * @throws std::runtime_error if there is a problem.
     */
    virtual void send(const std::string& name,
            const std::string& value,
            uint64_t timestamp);

    /**
     * Closes the TCP connection.
     */
    virtual void close();

private:
    bool connected_;
    std::string host_;
    uint16_t port_;

   Poco::Net::StreamSocket	socket_;
};

} /* namespace graphite */
} /* namespace cppmetrics */
#endif /* GRAPHITE_SENDER_TCP_H_ */
