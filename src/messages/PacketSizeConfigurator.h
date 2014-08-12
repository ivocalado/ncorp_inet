/*
 * PacketSizeConfigurator.h
 *
 *  Created on: Mar 31, 2013
 *      Author: ivocalado
 */

#ifndef PACKETSIZECONFIGURATOR_H_
#define PACKETSIZECONFIGURATOR_H_
#include "packets_m.h"

namespace ncorp {

/**
 * Esta classe serve como um helper para realizar a configuração de um pacote. Considera-se que os campos
 * * fonte
 * * destino
 * * num sequencia
 * * tipo do pacote
 *
 * são definidos posteriormente
 *
 */
class PacketSizeConfigurator {
public:
    PacketSizeConfigurator() {
    }
    void configure(CodedDataAck* packet) {
        if (!packet)
            return;
        uint64_t packet_size = 0;
        packet_size += 32; //relay addr
        packet_size += 16; //flow id
        packet_size += 16; //janela base
        packet_size += 16; //id geração
        packet_size += 16; //ack + ce + dbl
        packet_size += packet->getForwardSet().size() * 32; //Fs
        packet_size += packet->hasAck() ? 256 : 0; //ack coding
        packet_size += 256; //coeficientes
        packet_size += 1500 * 8; //payload
        packet->addBitLength(packet_size);

    }

    void configure(EtxProbe* packet) {
        if (!packet)
            return;
        uint64_t packet_size = 0;
        packet_size += 16; //num probes
        int probes = packet->getNeighboursArraySize();
        packet_size += probes * 32; //Probes address
        packet_size += probes * 8; //Probes
        packet->addBitLength(packet_size);
    }

    void configure(CodedAck* packet) {
        if (!packet)
            return;
        uint64_t packet_size = 0;
        packet_size += 16; //flow id
        packet_size += 16; //id geração
        packet_size += 16; //ack + ce + dbl
        packet_size += 256; //ack coding
        packet->addBitLength(packet_size);
    }

    void configure(CodedEAck* packet) {
        if (!packet)
            return;
        uint64_t packet_size = 0;
        packet_size += 16; //flow id
        packet_size += 16; //id geração
        packet->addBitLength(packet_size);
    }

    virtual ~PacketSizeConfigurator() {
    }
};

} /* namespace ncorp */
#endif /* PACKETSIZECONFIGURATOR_H_ */
