/*
 * CcackBaseline.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: ivocalado
 */

#include "CcackBaseline.h"
#include "Flow.h"
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <utility>
#include "messagecodes.h"
#include "Ncorp.h"
#include "helper-functions.h"

namespace ncorp {

CcackBaseline::CcackBaseline(Ncorp *mNcorp) :
        mainNcorp(mNcorp) {

    myNetAddr = mainNcorp->getMyNetAddr();
    useSingleGeneration = mainNcorp->par("CCACKUseSingleGeneration");
    generation_size = mainNcorp->par("CCACKGenerationSize");
    packet_size = mainNcorp->par("CCACKPayloadSize");
    alfa = 5. / 6.;
    beta = 1. / 6.;
}

CcackBaseline::~CcackBaseline() { //
}

bool CcackBaseline::hasFlow(uint16_t flowId) {
    return std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> flow) {
                return flowId == flow->getId();
            }) != flows.end();
}

IPv4Address CcackBaseline::getDestByFlow(uint32_t flowId) {
    if (!hasFlow(flowId))
        return IPv4Address::UNSPECIFIED_ADDRESS;
    return (*std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> flow) {
                return flowId == flow->getId();
            }))->getDestination();
} //Retorna o destino de um determinado fluxo. Se o fluxo nao for encontrado retorna LAddress::L3NULL

IPv4Address CcackBaseline::getSrcByFlow(uint32_t flowId) {
    if (!hasFlow(flowId))
        return IPv4Address::UNSPECIFIED_ADDRESS;
    return (*std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> flow) {
                return flowId == flow->getId();
            }))->getSource();
} //Retorna a fonte de um determinado fluxo. Se o fluxo nao for encontrado retorna LAddress::L3NULL

void CcackBaseline::handleNewDblValue(IPv4Address neighbour, double dblValue,
        simtime_t now) {
    neighboursDbl[neighbour] = make_pair(dblValue, now);
} //Atualiza o valor do dbl para o vizinho

//Manipula um pacote eack. Retorna o endereço do destino a ser roteado ou L3NULL caso tenha atingido o destino
IPv4Address CcackBaseline::handleEAckPkt(CodedEAck* packet) {
    auto flowId = packet->getFlowId();
    auto generationId = packet->getGenerationId();
    auto flow = find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> f) {
                return f->getId() == flowId;
            });
    if (flow != flows.end()) {
        (*flow)->handleEAck(generationId);
        auto src = getSrcByFlow(flowId);
        delete packet;
        return src != myNetAddr ? src : IPv4Address::UNSPECIFIED_ADDRESS; //Se for a fonte para o roteamento
    }
    delete packet;
    return IPv4Address::UNSPECIFIED_ADDRESS;
}

NcorpPacket* CcackBaseline::handleCodedDataAtDownstream(CodedDataAck* packet,
        IPv4Address from, simtime_t now) {

    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::handleCodedDataAtDownstream Begin\n");
    handleNewDblValue(from, packet->getDbl(), now); //Atualiza o valor do dbl do nó

    auto flowId = packet->getFlowId();
    auto generationId = packet->getGenerationId();
    auto baseWindow = packet->getBaseWindow();
    auto codedPacket = packet->getPayload();

    auto flow = find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> f) {
                return f->getId() == flowId;
            }); //Recupera o fluxo

    NcorpPacket* result = NULL;
    if (flow != flows.end()) {
        result = (*flow)->handleCodedPacket(generationId, baseWindow,
                codedPacket, packet->getPayloadSize()); //Se o fluxo existir, chama o manipulador
        (*flow)->printInfo();
    } else {
        debugprintf(stderr, LOG_LEVEL_2, "Criando novo fluxo\n");
        std::shared_ptr<ncorp::Flow> newFlow(
                new Flow(mainNcorp, myNetAddr, packet->getFlowId(),
                        packet->getFlowSrcAddr(), packet->getFlowDstAddr(),
                        myNetAddr == packet->getFlowDstAddr() ?
                                RECEIVER : RELAY, generation_size, packet_size,
                        useSingleGeneration));
        flows.push_back(newFlow);
        result = newFlow->handleCodedPacket(generationId, baseWindow,
                codedPacket, packet->getPayloadSize());
        newFlow->printInfo();
    }
    delete packet; //Delete unnecessary packets

    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::handleCodedDataAtDownstream End\n");
    return result;
}

void CcackBaseline::handleCodedDataAtUpstream(CodedDataAck* packet,
        IPv4Address from, simtime_t now) {
    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::handleCodedDataAtUpstream Begin\n");
    handleNewDblValue(from, packet->getDbl(), now); //Atualiza o valor do dbl do nó

    auto flowId = packet->getFlowId();
    auto generationId = packet->getGenerationId();
    auto codedPacket = packet->getPayload();
    auto nodeId = from;

    auto flow = std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> flow) {
                return flow->getId() == flowId;
            });

    if (flow != flows.end() && packet->hasAck()) {
            (*flow)->handleAckCoding(generationId, packet->getAckVector(),
                                nodeId.getInt());
        (*flow)->printInfo();
    }

    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::handleCodedDataAtUpstream End\n");
    delete packet;
}

void CcackBaseline::handleCodedAckAtUpstream(CodedAck* packet,
        IPv4Address from) {

    auto flowId = packet->getFlowId();
    auto generationId = packet->getAckGenerationId();
    auto ackCoding = packet->getAckVector();
    auto nodeId = from;

    auto flow = std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> flow) {
                return flow->getId() == flowId;
            });

    if (flow != flows.end()) {
        (*flow)->handleAckCoding(generationId, ackCoding, nodeId.getInt());
        (*flow)->printInfo();
    }

    delete packet;
}

void CcackBaseline::pushRawBlock(IPv4Address from, IPv4Address dest,
        uint16_t flowId, std::vector<uint8_t> payload) {
    auto it = std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> entry) {
                return entry->getId() == flowId;
            });
    std::shared_ptr<ncorp::Flow> flow;
    if (it != flows.end()) {
        flow = *it;
    } else {
        flow = std::shared_ptr < ncorp::Flow
                > (new Flow(mainNcorp, myNetAddr, flowId, from, dest, SENDER,
                        generation_size, packet_size, useSingleGeneration));
        flows.push_back(flow);
    }
    flow->pushRawBlock(payload);
    flow->printInfo();
}

bool CcackBaseline::flushStream(uint16_t flowId) {
    auto it = std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> entry) {
                return entry->getId() == flowId;
            });
    if (it == flows.end()) {
        return false;
    }

    (*it)->flush();

    return true;
}

CodedDataAck* CcackBaseline::replacePacket(uint16_t flowId) {
    auto flow = std::find_if(flows.begin(), flows.end(),
            [flowId](std::shared_ptr<ncorp::Flow> entry) {
                return entry->getId() == flowId;
            });

    return flow == flows.end() ? NULL : (*flow)->generateNewPacket();
}

/**
 * Retorna o próximo pacote a ser transmitido entre todos os fluxos ou NULL
 */
CodedDataAck* CcackBaseline::nextPacketToTransmit() {
    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::nextPacketToTransmit Begin\n");
    debugprintf(stderr, LOG_LEVEL_2, "Selecionando fluxo para transmissao\n");
    auto flow = nextFlowToTransmit();

    CodedDataAck* packet = flow ? flow->generateNewPacket() : NULL;
    if (packet) {
        double dbl = 0;
        for_each(flows.begin(), flows.end(),
                [&dbl](std::shared_ptr<ncorp::Flow> flow) {
                    dbl += flow->calculateDifferentialBacklog();
                });
        packet->setDbl(dbl);
        packet->setForwardSet(mainNcorp->findCandidateSet(packet->getFlowDstAddr()));
        debugprintf(stderr, LOG_LEVEL_2, "Pacote retornado!\n");
    } else {
        debugprintf(stderr, LOG_LEVEL_2, "NÃO ha pacote a ser transmitido!\n");
    }

    if (flow)
        flow->printInfo();

    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::nextPacketToTransmit End\n");
    return packet;
}

void CcackBaseline::retrieveDecodedBlock(uint16_t& flowId,
        std::shared_ptr<std::vector<uint8_t> > vec) {
    //Recupera o bloco de dados originado do processo de decodificação
    for (auto flow = flows.begin(); flow != flows.end(); flow++)
        if ((*flow)->isReadyToPushUp())
            return (*flow)->retrieveDecodedBlock(flowId, vec);
    flowId = -1;
}

std::shared_ptr<ncorp::Flow> CcackBaseline::nextFlowToTransmit() {
    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::nextFlowToTransmit Begin\n");
    double deltaN = 0;
    if (flows.empty())
        return std::shared_ptr<ncorp::Flow>();

    auto transmittionTime = mainNcorp->getTransmissionTime();
    auto neighboursSize = neighboursDbl.size();
    std::list<IPv4Address> neighboursToRemove;
    auto now = simTime();

    std::for_each(neighboursDbl.begin(), neighboursDbl.end(),
            [&deltaN, transmittionTime, neighboursSize, &neighboursToRemove, now](std::pair<IPv4Address, std::pair<double, simtime_t> > entry) {
                if((entry.second.second + neighboursSize*transmittionTime) >= now )
                    deltaN += entry.second.first;
                else
                    neighboursToRemove.push_back(entry.first);
            });

    std::for_each(neighboursToRemove.begin(), neighboursToRemove.end(), [this](IPv4Address addr){
        neighboursDbl.erase(addr);
//            debugprintf(stderr,
//                    "[Node = %s] CcackBaseline::REMOCAO -- Vizinho removido: %s\n", mainNcorp->getMyNetAddr().str(true).c_str(), addr.str(true).c_str());

    });


    std::srand(unsigned(std::time(0)));
    std::random_shuffle(flows.begin(), flows.end()); // Here we shuffle the flows to retrieve the flows randomly.

    auto flow = std::find_if(flows.begin(), flows.end(),
            [deltaN, this, neighboursSize](std::shared_ptr<ncorp::Flow> flow) {
                double dbl = flow->calculateDifferentialBacklog();

                if(dbl > 0) {
                    double dblRel = dbl / (dbl + deltaN);
                    auto result = flow->getCredits() + beta + alfa * dblRel;
                    flow->setCredits(result);

                    if(flow->getCredits() > 0) {
                        flow->setCredits(flow->getCredits() - 1);
                        return true;
                    }
                }
                return false;
            });

    auto result = std::shared_ptr<ncorp::Flow>();
    if(flows.end() == flow) {
        debugprintf(stderr, LOG_LEVEL_2, "Nenhum fluxo selecionado!\n");
    } else {
        debugprintf(stderr, LOG_LEVEL_2, "Fluxo selecionado!\n");
        result = *flow;
    }
    debugprintf(stderr, LOG_LEVEL_2, "CcackBaseline::nextFlowToTransmit End\n");
    return result;
}

} /* namespace ncorp */
