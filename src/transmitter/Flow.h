/* -*- mode:c++ -*- ********************************************************
 * file:        Flow.h
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
 * description: An abstraction of a Flow
 **************************************************************************/
#ifndef FLOW_H_
#define FLOW_H_

#include <memory>
#include <set>
#include <vector>
#include <deque>
#include <functional>

#include "IPv4Address.h"
#include "Generation.h"

//Packets
#include "packets_m.h"
#include "messagecodes.h"

class Ncorp;

namespace ncorp {

/**
 * @brief This class represents an abstraction of a flow. It is created automatically when an application
 * sends a packet to network layer or when a relay receives a packet from a new flow. On the other hand,
 * the flow can be removed after an period of inactivity.
 *
 * The flow object is responsible to manage the several generations. In other words, in this module
 * we implement the pipeline approach of ccack
 *
 *
 * @ingroup netwLayer
 * @author Ivo Calado
 *
 * ported to Mixim 2.0 by Ivo Calado
 **/
class Flow {

public:

    /**
     * @brief The constructor for the Flow object.
     *
     * Params description:
     *
     * nodeId           - a uint32_t used as the node identifier. It is used at any node as a seed for construction of has matrices.
     * id               - the flow id
     * source           - the address of flow source
     * destination      - the address of flow destination
     * role             - specifies if the current node is the sender, receiver or if it is the relay
     * generation_size  - defines the size of a generation in terms of packets
     * symbol_size      - defines the size of data payload
     * window_size      - defines a maximum number of generations that can be active at time. If window_size = 1, represents the traditional CCACK
     */
    Flow(Ncorp* mainNcorp, IPv4Address nodeId
         , uint16_t id
         , IPv4Address source
         , IPv4Address destination
         );

    virtual ~Flow();

    /**
     * @brief Push a raw block of data to be transmitted. It is copied to an internal buffer and when
     * it achieve the water mark (generation_size * symbol_size) a generation is created
     *
     * Params description:
     * payload - the uncoded vector sent by the application
     *
     * This method performs the following steps:
     * 1) Copies the block to the internal buffer
     * 2) Checks if the internal block achieve the water mark (generation_size * symbol_size). If so,
     *    creates a new generation and insert the new generation in the list of active generations.
     */
    void pushRawBlock(std::vector<uint8_t> payload);

    /**
     * Realiza um flush no fluxo criando uma geração com o buffer restante
     */
    void flush();

    /**
     * @brief Retrieves the flow id
     *
     * @return the id
     */
    uint16_t getId() const;

    /**
     * @brief Calculates the Differential Backlog of a flow.
     *
     * This method performs the following steps:
     * 1) Iterate among all generations of the flow
     * 2) Checks if the current generation is on the window_size range
     * 3) If so, calls the method to calculate the DBL in the generation
     *
     * @return the DBL.
     */
    double calculateDifferentialBacklog();

    /**
     * @brief Retrieve the credits for a flow
     *
     * @return the credit previously calculated
     */
    double getCredits() const;

    /**
     * @brief Set the current credit value for the flow
     *
     * Params description:
     * credits - to be set
     */
    void setCredits(double credits);

    /**
     * @brief Create a new packet
     *
     * This method performs the following steps:
     * 1) Checks the role of the flow. If it is a Sender or relay continue
     * 2) Creates a NcorpCodedDataAckPkt packet and fill the parameters
     *
     * @return the packet to be transmitted or NULL if currently there is no packet to be sent
     */
    CodedDataAck* generateNewPacket();

    /**
     * @brief Handling the arrive of a new Ack Coding vector
     *
     * Params description:
     *
     * generationId -
     * ackCoding - the ack coding vector. We use the shared_ptr structure to avoid memory leaks
     * nodeId - the nodeId of transmitter. Through this parameter we can recreate the hash matrices
     *
     * This method performs the following steps:
     * 1) Retrieves the generation
     * 2) Call the respective AckCoding handler
     * 3)
     */
    void handleAckCoding(uint16_t generationId,
            std::vector<uint8_t> ackCoding, uint32_t nodeId);

    /**
     * @brief Handling a coded packet. This method is called in a downstream node when it receives a new data packet
     *
     * Params description:
     *
     * generationId - no description is needed
     * baseWindow - defines the oldest generation still unconfirmed by the receiver
     * codedPacket - the packet plus code coefficients
     *
     *
     * This method performs the following steps:
     *
     * If it is a sender:
     * 1) exit returning NULL
     *
     * If it is a relay:
     * 1) Update the base window
     * 2) Remove all generations older than base window
     * 3) Retrieve the generation which its id is equals to generationId
     * 4) If the retrieved generation is the base window exit and return NULL. Otherwise, transmit a NcorpCodedAckPkt
     *
     * If it is a receiver
     * 1) Retrieve the searched generation
     * 2) Pass the block to the generation
     * 3) If the retrieve generation is the base windows and it is complete (it has received the required number of coded packet)
     * 3.1) Decode all the generation
     * 3.2) Pass the decoded block to the output buffer
     * 3.3) Remove the generation
     * 3.4) Increment the base window
     * 3.5) return a NcorpEAckPkt
     *
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
    NcorpPacket* handleCodedPacket(uint16_t generationId, uint16_t baseWindow,
            std::shared_ptr<std::vector<uint8_t> > codedPacket, size_t payloadSize);

    /**
     * @brief Handle a NcorpEAckPkt packet.
     *
     * Params description:
     *
     * generationId - The oldest generation still not acked.
     *
     * This method performs the following steps:
     * 1) update the Base Window as the new generationId
     * 2) Remove all generations older the generationId
     */
    void handleEAck(uint16_t generationId);

    /**
     * @brief Retrieve the flow destination address
     *
     * @return the destination address
     */
    IPv4Address getDestination() const;

    /**
     * @brief Retrieve the flow source address
     *
     * @return the source address
     */
    IPv4Address getSource() const;

    /**
     * @brief Retrive the role of the node regarding the flow
     *
     * @return SENDER, RELAY or RECEIVER
     */
    Role getRole() const;

    /**
     * @brief Checks if the flow is able to push up data to the application layer.
     *
     * @return true if there is data to be sent to application layer
     */
    bool isReadyToPushUp(); //Retorna verdadeiro se houver pacote a ser enviado para a camada de app

    /**
     * @brief Retrieve the output data from the internal output buffer. Also, sets the flowId as the local Flow identification.
     */
    void retrieveDecodedBlock(uint16_t& flowId,
            std::shared_ptr<std::vector<uint8_t> > vec); //Recupera o bloco de dados originado do processo de decodificação

    bool operator ==(Flow & other) {
        return id == other.id;
    }
    bool operator <(const Flow & x) const
    {
        return id < x.id;
    }

    virtual void processTimer(cMessage* msg);

    void printInfo();
protected:
    Ncorp* mainNcorp;

    //The node id. This field is used to generate the hash matrices in ccack
    IPv4Address nodeId;

    //Defines the flow identification
    uint16_t id;

    //Defines the source address
    IPv4Address source;

    //Defines the destination address
    IPv4Address destination;

    //Defines the role of node regarding the flow
    Role role;

    //Defines the generation size in terms of packets
    uint32_t generation_size;

    //Defines the maximum payload size of native packets
    uint32_t symbol_size;

    //The input data queue.
    std::deque<uint8_t> rawData;

    //The outpur data queue
    std::vector<uint8_t> rawDataOutput;

    //Retrieve the next available generation id
    uint16_t nextGenerationId();

    //Return the maximum window size
    uint16_t window_size;

    bool useSingleGeneration;//if true, only a single generation is allowed

    uint16_t calculateWindowSize();


    //Define the current base window
    uint16_t leftBoundGenerationId;

    //The credits counter
    double credits;

    //The list of current active generations
    std::set<std::shared_ptr<Generation>, std::function<bool (const std::shared_ptr<Generation>&, const std::shared_ptr<Generation>&)> > generations;

    // The current generation timeout
    simtime_t gto;

    // The current Smoth Generation Trip Time
    simtime_t sgtt;

    // The current gttvar
    simtime_t gttvar;

    double gto_alpha;

    double gto_beta;


    /**
     * @brief Retrieve the current generation. It checks among all generation of window size, which of them has packets to transmit,
     *  where the older generations have higher priorities than newer.
     *
     *  @return the current generation
     */
    std::shared_ptr<Generation> getCurrentGeneration();

    /**
     * Updates the gto-related vars according to generationId
     */
    void updateGto(uint16_t generationId);

};

} /* namespace ncorp */
#endif /* FLOW_H_ */
