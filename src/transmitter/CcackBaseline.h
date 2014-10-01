/* -*- mode:c++ -*- ********************************************************
 * file:        CcackBaseline.h
 *
 * author:      Ivo Calado
 *
 * copyright:   (C) 2013 Embedded Systems and Pervasive Computing Laboratory
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 *
 ***************************************************************************
 * part of:     protocol ncorp
 * description: An implementation of CCACK mechanism
 **************************************************************************/

#ifndef CCACKBASELINE_H_
#define CCACKBASELINE_H_

#include <list>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

#include "Flow.h"
#include "IPv4Address.h"

//Packet-related includes
#include "packets_m.h"

class MyNetworkLayer;

namespace ncorp {

/**
 * @brief An implementation of a Pipelined version of CCACK algorithm
 *
 *
 * @ingroup netwLayer
 * @author Ivo Calado
 *
 * ported to Mixim 2.0 by Ivo Calado
 **/
class CcackBaseline {

public:

    /**
     * @brief The constructor for the CCACK object. Basically it receives all properties required to work.
     *
     * Params description:
     *
     * myNetAddr        - a uint32_t used as the node identifier
     * useSingleGeneration     - true, represents the traditional CCACK with only one active generation
     * generation_size  - defines the size of a generation in terms of packets
     * app_pkt_size     - defines the size of data payload
     */
    CcackBaseline(Ncorp *mainNcorp);

    virtual ~CcackBaseline();

    /**
     * @brief This method is called by a higher protocol to retrieve the next packet to transmit.
     * It will be select which flow among all current flows is able to transmit
     *
     * This method performs the following steps:
     *
     * 1) retrieve the flow able to transmit (@see nextFlowToTransmit)
     * 2) generate a new packet
     * 3) set in the packet the DBL value
     *
     * @return The packet to be transmitted. If no flow is able to transmit it will be returned NULL
     */
    CodedDataAck* nextPacketToTransmit();

    /**
     * Replace the packet for a newer version. If the packet is NULL, replaces the packet
     */
    CodedDataAck* replacePacket(uint16_t flowId);

    /**
     * @brief This method is used to pass data from application to be transmitted. For now, we do not expect no insane data.
     * When this method is called in a first time for a flow, it creates internally the flow structure required to manage
     * the flow.
     *
     * This method performs the following steps:
     *
     * 1) Find the required flow. If it doesn't exist, create a new entry.
     * 2) Push the payload to the flow.
     *
     * Params description:
     *
     * from - the source address
     * dest - the target address
     * flowId - a flow identifier. When the application wants to create a new flow it chooses randomly a new identifier for the flow
     * payload - the payload to be sent. It can have different size. It is passed for a internal buffer and just this buffer surpass
     * the app_pkt_size it transmit, similar to the water mark in TCP.
     *
     * TODO: in the next versions we aim implement a flush method.
     */
    void pushRawBlock(IPv4Address from, IPv4Address dest,
            uint16_t flowId, std::vector<uint8_t> payload);

    /**
     * Realiza um flush no fluxo cujo id eh passado por parametro. A ideia é forçar o envio de gerações incompletas.
     * Se o flowId não indicar nenhum fluxo válido retorna-se falso
     */
    bool flushStream(uint16_t flowId);

    /**
     * @brief This method handles the arriving of a new NcorpCodedDataAckPkt at a DOWNSTREAM node.
     * Since we are talking about a opportunistic routing protocol, each packet transmitted by a
     * relay can be both received in the forward path but also in backward. The structure below
     * shows the relationship among all the different nodes in the network.
     *
     *
     *    +-----------+      +-----------+      +------------+      +------------+      +------------+
     *    |  source   |<---->| upstream  |<----+|transmitter |+---->| downstream |<---->|  receiver  |
     *    +-----------+      |   relay   |      |   relay    |      |   relay    |      +------------+
     *                       +-----------+      +------------+      | (cur node) |
     *                                                              +------------+
     *
     * Params description:
     *
     * packet - the packet transmitted by the transmitter relay. A NcorpCodedDataAckPkt is composed basically by the
     * following attributes: (there are others but not used by ccack)
     *
     * - Source address
     * - Receiver address
     * - Transmitter relay address
     * - Flow Id: identifies the transmitted flow
     * - Generation Id: identifies the current generation
     * - Base Window Id: identifies identifies the oldest generation yet not acknowledged by the receiver
     * - Differential backlog: used by CCACK to determine, in the rate control mechanism, which flow is able to
     *   transmit
     * - Ack vector: IT IS IGNORED BY A DOWNSTREAM NODE
     * - Encoded payload + Coding Coefficients
     *
     *
     * This method performs the following steps:
     * 1) Calls 'handleNewDblValue' to update the DBL value
     * 2) Find the required flow. If it doesn't exist, create a new entry.
     * 3) Calls the handler
     *
     * @return the current node can return a packet in the following situations:
     *
     * 1) A NcorpCodedAckPkt packet if the node is running with more than one active generation (window_size > 1) and
     *  the packet received is not the current generation (where the ack coding could be delayed to be transmitted
     *  together with a new data packet)
     *
     * 2) A NcorpCodedAckPkt packet if the node is actually a receiver and the generation is not complete yet (more packets are needed)
     *
     * 3) A NcorpEAckPkt packet if the node is actually a receiver and the generation HAS BEEN COMPLETED.
     */
    NcorpPacket* handleCodedDataAtDownstream(CodedDataAck* packet, IPv4Address from, simtime_t now);


    /**
     * @brief This method handles the arriving of a new NcorpCodedDataAckPkt at a UPSTREAM node.
     * Since we are talking about a opportunistic routing protocol, each packet transmitted by
     * a relay can be both received in the forward path but also in backward. The structure below
     * shows the relationship among all the different nodes in the network.
     *
     *
     *    +-----------+      +-----------+      +------------+      +------------+      +------------+
     *    |  source   |<---->| upstream  |<----+|transmitter |+---->| downstream |<---->|  receiver  |
     *    +-----------+      |   relay   |      |   relay    |      |   relay    |      +------------+
     *                       | (cur node)|      +------------+      +------------+
     *                       +-----------+
     * Params description:
     *
     * packet - the packet transmitted by the transmitter relay. A NcorpCodedDataAckPkt is composed basically by the
     * following attributes: (there are others but not used by ccack)
     *
     * - Source address
     * - Receiver address
     * - Transmitter relay address
     * - Flow Id: identifies the transmitted flow
     * - Generation Id: identifies the current generation
     * - Base Window Id: identifies identifies the oldest generation yet not acknowledged by the receiver
     * - Ack vector: used to confirm the receiving of previous packets
     * - Encoded payload + Coding Coefficients: IT IS IGNORED BY A UPSTREAM NODE
     *
     * This method performs the following steps:
     * 1) Check if the packet contains an ACK vector
     * 2) Find the required flow. If it doesn't exist, create a new entry.
     * 3) Calls the respective AckCoding handler
     *
     */
    void handleCodedDataAtUpstream(CodedDataAck* packet, IPv4Address from, simtime_t now);

    /**
     * @brief This method handles the arriving of a new NcorpCodedAckPkt at a UPSTREAM node.
     * Since we are talking about a opportunistic routing protocol, each packet transmitted by
     * a relay can be both received in the forward path but also in backward. The structure below
     * shows the relationship among all the different nodes in the network.
     *
     *
     *    +-----------+      +-----------+      +------------+      +------------+      +------------+
     *    |  source   |<---->| upstream  |<----+|transmitter |+---->| downstream |<---->|  receiver  |
     *    +-----------+      |   relay   |      |   relay    |      |   relay    |      +------------+
     *                       | (cur node)|      +------------+      +------------+
     *                       +-----------+
     * Params description:
     *
     * packet - the packet transmitted by the transmitter relay. A NcorpCodedAckPkt is composed basically by the
     * following attributes: (there are others but not used by ccack)
     *
     * - Source address
     * - Receiver address
     * - Flow Id: identifies the transmitted flow
     * - Generation Id: identifies the current generation
     * - Ack vector: used to confirm the receiving of previous packets
     *
     * This method performs the following steps:
     * 1) Find the required flow. If it doesn't exist, create a new entry.
     * 2) Calls the respective AckCoding handler
     *
     */
    void handleCodedAckAtUpstream(CodedAck* packet, IPv4Address from);

        /**
     * This method handles the arriving of a new NcorpEAckPkt packet. When a node hear a EAck packet it checks if
     * there is a register for the passed flow. If so, it cancels all generations prior the informed in the packet.
     * Only the packet select by the sender (its address is defined in the destination field.
     *
     * Params description:
     *
     * packet - the packet transmitted by the transmitter relay. A NcorpCodedAckPkt is composed basically by the
     * following attributes: (there are others but not used by ccack)
     *
     * - Source address: defines the node which generate the packet (can be the receiver or a relay)
     * - Receiver address: defines the packet responsible to forward this packet to sender (it will be the new sender)
     * - Flow Id: identifies the transmitted flow
     * - Generation Id: confirms all generations below this.
     *
     * This method performs the following steps:
     * 1) Retrieve the specified flow
     * 2) Calls the respective handler for EAck packets
     *
     * @return If the node is not the sender, it returns the Flow source address (through this, we can retrieve the
     * next neighbor to forward the packet through ETX metric)
     *
     */
    IPv4Address handleEAckPkt(CodedEAck*);

    /**
     * @brief retrieve a decoded block. It checks all flows and retrieve the first flow which the current node
     *  is the receiver and that has decoded block.
     *
     *  Params description:
     *
     *  flowId - defines the identification of the flow to be retrieved
     *  vec - represents the output buffer
     *
     *  Both parameters are not defined prior the calling but instead are used as output
     *
     * This method performs the following steps:
     * 1) Iterate among all flows checking which flow is able to push up data
     * 2) Calls the 'retrieveDecodedBlock' to retrieve the uncoded data block
     */
    void retrieveDecodedBlock(uint16_t& flowId,
                std::shared_ptr<std::vector<uint8_t> > vec);

    /**
     * @brief Check if the node has the required flow
     *
     * @return true if the node has the specified flow
     */
    bool hasFlow(uint16_t flowId);


    /**
     * @bried getDestByFlow and getSrcByFlow represent two helper functions to retrieve information about a specified
     *  flow.
     *
     * @return the required flow address. If the flow doesn't exist in the current node a LAddress::L3NULL is returned
     */
    IPv4Address getDestByFlow(uint32_t flowId);
    IPv4Address getSrcByFlow(uint32_t flowId);
protected:

    Ncorp* mainNcorp;

    //List of current active flows in the node
    std::vector<std::shared_ptr<ncorp::Flow> > flows;

    //Local address
    IPv4Address myNetAddr;

    //Generation size defined in terms of native packets
    uint32_t generation_size;

    //Max size of a native packet in bytes
    uint32_t packet_size;

    /* A map used to store all differential backlog values from the neighbours.
     *
     * The map is composed by the following attributes:
     *
     * - neighbour address (L3Type)
     * - the dbl value (double)
     * - the time when the measure was took
     */
    std::map<IPv4Address, std::pair<double, simtime_t> > neighboursDbl;

    /**
     * @brief handle a new entry for DBL values. Basically, it replaces a previous entry
     */
    void handleNewDblValue(IPv4Address neighbour, double dblValue, simtime_t now); //Atualiza o valor do dbl para o vizinho

    /**
     * Specifies the alfa and beta values for CCACK algorithm
     */
    double alfa;
    double beta;

    size_t num_created_flows_as_origin;//Especifica o número de fluxos criados como uma origem
    size_t num_created_flows_as_relay;//Especifica o número de fluxos criados como relay
    size_t num_created_flows_as_receiver;//Especifica o número de fluxos criados como receiver

    /**
     * @brief Defines what Flow is able to transmit. It is used internally by the nextPacketToTransmit
     *
     * This method performs the following steps:
     * 1) Calculate the DBL value for all neighbors
     * 2) Iterate in the local flows checking which flow attend the requirements for transmission
     *    (see CCACK rate control mechanism)
     *
     * @param increment. If true, increment the flow credit card by 1/K * (beta + alfa * dblRel), where K represents the number
     * @return the flow allowed to transmit or false
     */
    std::shared_ptr<ncorp::Flow> nextFlowToTransmit();
};

} /* namespace ncorp */
#endif /* CCACKBASELINE_H_ */
