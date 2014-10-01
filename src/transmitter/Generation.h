/* -*- mode:c++ -*- ********************************************************
 * file:        Generation.h
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
 * description: An abstraction of a Generation
 **************************************************************************/

#ifndef GENERATION_H_
#define GENERATION_H_

#include "Buffers.h"
#include <kodo/rlnc/full_vector_codes.hpp>
#include <kodo/cached_symbol_decoder.hpp>
#include <kodo/empty_decoder.hpp>
#include <vector>
#include <memory>
#include "messagecodes.h"
#include <utility>
#include <omnetpp.h>
#include <tuple>
namespace kodo {

/// @brief The symbol info stack defined below can be used to split an
///        incoming encoded payload into the encoded
///        symbol data and symbol id (the RLNC encoding vector).
///
///        You can check the API of the cached_symbol_decoder to
///        see how to access information about the coded symbol.
///
///        The info stack is created in such a way that it
///        is compatible and can decode symbols produced by a
///        full_rlnc_encoder. In case you wish to use a similar approach
///        for a different codec stack you should ensure that the
///        layers in the two stacks are compatible (i.e. at least "Payload
///        API" to "Codec API" are the same).
template<class Field>
class symbol_info_decoder: public // Payload API
payload_decoder<
// Codec Header API
        systematic_decoder<
                symbol_id_decoder<
                // Symbol ID API
                        plain_symbol_id_reader<
                        // Codec API
                                cached_symbol_decoder< // <-- Cached symbol decoder
                                        empty_decoder<
                                        // Coefficient Storage API
                                                coefficient_info<
                                                // Storage API
                                                        storage_bytes_used<
                                                                storage_block_info<
                                                                // Finite Field API
                                                                        finite_field_info<
                                                                                Field,
                                                                                // Factory API
                                                                                final_coder_factory_pool<
                                                                                // Final type
                                                                                        symbol_info_decoder<
                                                                                                Field> > > > > > > > > > > > {
};

}

namespace ncorp {

/**
 * @brief This class represents an abstraction of a Generation. It is created automatically when an flow
 * needs to transmit a new data block
 *
 * The flow object is responsible to manage the several generations.
 *
 *
 * @ingroup netwLayer
 * @author Ivo Calado
 *
 * ported to Mixim 2.0 by Ivo Calado
 **/
class Generation {

public:

    /**
     * @brief The constructor for the Generation object. This constructor is used only by the sender since it passes
     * a uncoded rawblock to construction
     *
     * Params description:
     *
     * nodeId           - a uint32_t used as the node identifier. It is used at any node as a seed for construction of has matrices.
     * id               - the generation id
     * role             - specifies if the current node is the sender, receiver or if it is the relay
     * symbols          - defines the size of a generation in terms of packets
     * symbol_size      - defines the size of data payload
     * rawBlock         - The payload to be encoded
     * payload          - indica a carga util real
     */
    Generation(uint32_t nodeId, uint16_t id, /*Role role, */uint32_t symbols,
            uint32_t symbol_size, std::vector<uint8_t> rawBlock, size_t payload);

    /**
     * @brief The alternative constructor for the Generation object. This constructor is used by both relays and receivers nodes
     *
     * Params description:
     *
     * nodeId           - a uint32_t used as the node identifier. It is used at any node as a seed for construction of has matrices.
     * id               - the generation id
     * role             - specifies if the current node is the sender, receiver or if it is the relay
     * symbols          - defines the size of a generation in terms of packets
     * symbol_size      - defines the size of data payload
     * payload          - indica a carga util real
     */
    Generation(uint32_t nodeId, uint16_t id, Role role, uint32_t symbols,
            uint32_t symbol_size, size_t payload);

    /**
     * @brief This method is used to push encoded data to the generation. This method is used only in relays nodes and receivers nodes
     *
     * Params description:
     *
     * payload          - the encoded payload. Since we are using the Kodo lib, this payload encloses by encoded block and its coding coefficients
     */
    void pushEncodedData(std::shared_ptr<std::vector<uint8_t> > payload);

    /**
     * @brief Handle an Ack encoding vector.
     *
     * Params description:
     *
     * payload          - Push an Ack Coding Vector to the generation
     * seed             - Node identification
     */
    void pushAckCoding(std::vector<uint8_t> payload, uint32_t nodeId); //

    /**
     * @brief Check if the generation is already completed
     *
     * @return true if the generation has already received all the required coded packets.
     */
    bool isComplete();

    /**
     * @brief Generate a new encoded packet
     *
     * This method generate an encoded packets. The encoded used depends of the current role
     * pair<coefficients, payload>
     * @return the encoded packet
     */
    std::tuple<std::vector<uint8_t>, std::shared_ptr<std::vector<uint8_t> >, bool> generateEncodedPacket();

    /**
     * @brief Retrieve the ack vector based on the current decoder payload
     *
     * @return the ack vector
     */
    std::vector<uint8_t> retrieveCurrentAckVector();

    virtual ~Generation();

    size_t getPayloadSize() const;

    /**
     * @brief Calculate the differential backlog of the current generation
     *
     * @return the differential backlog
     */
    double calculateDifferentialBacklog();

    /**
     * @brief Retrieve the generation id
     *
     * @return the id
     */
    uint16_t getId() const;

    /**
     * @brief Check if the generation is able to transmit the current moment
     *
     * @return true if the generation is able to transmit
     */
    bool isTransmitting();

    /**
     * @brief Return the decoded block
     *
     * This method must be called only after the 'isComplete' method returns true.
     * Otherwise it will return an empty buffer
     *
     * @return the decoded block
     */
    std::shared_ptr<std::vector<uint8_t> > getDecodedBlock();

    cMessage* getTimeoutMsg();

    void printInfo();

    void startTransmit(); //Marca o inicio da transmissão, ajustando o t0

    void stopTransmit(); //Marca o fim da transmissão, ajustando o t_eack

    simtime_t calculateDT(); //Calcula DT, tempo de entrega

protected:
    uint32_t nodeId;
    uint16_t id;
    typedef kodo::full_rlnc_encoder<fifi::binary8> rlnc_encoder;
    typedef kodo::full_rlnc_decoder<fifi::binary8> rlnc_decoder;
    typedef kodo::symbol_info_decoder<fifi::binary8> rlnc_info_decoder; //possibilita a recuperacao do vetor de coeficientes
    uint32_t generation_size;
    uint32_t packet_size;
    boost::shared_ptr<kodo::full_rlnc_encoder<fifi::binary8> > encoder;
    rlnc_decoder::pointer decoder;
    rlnc_encoder::factory encoder_factory;
    rlnc_decoder::factory decoder_factory;
    rlnc_info_decoder::factory info_decoder_factory;
    std::shared_ptr<std::vector<uint8_t> > data_out;
    EncodeBuffers ebuffers;
    Role role;
    cMessage* timeout;
    size_t payloadSize; //Define a carga útil real de payload. Em geral esse valor é equivalente a geração completa,
                    //com a exceção de quando a geração for proveniente de um flush
    bool operator <(const Generation & x) const {

        return id < x.id;
    }
    bool markNextPktAsInovative;

    simtime_t t0; //Indica o momento que a geracao começou a ser transmitida
    simtime_t t_eack; //Indica o momento em que se recebeu o E-ack que confirme a geração
};

} /* namespace ncorp */
#endif /* GENERATION_H_ */
