/*
 * Flow.cpp
 *
 *  Created on: Mar 17, 2013
 *      Author: ivocalado
 */

#include "Flow.h"
#include <algorithm>
#include "messagecodes.h"
#include "Generation.h"
#include <iostream>
using namespace std;
#include "Ncorp.h"

//
namespace ncorp {
//
//using namespace std;
//
Flow::Flow(Ncorp* mNcorp, IPv4Address nId, uint16_t ident,
        IPv4Address src, IPv4Address dest, Role rl, uint32_t gen_size,
        uint32_t symb_size, bool usg) :
        mainNcorp(mNcorp), nodeId(nId), id(ident), source(src), destination(
                dest), role(rl), generation_size(gen_size), symbol_size(
                symb_size), window_size(1), useSingleGeneration(usg), leftBoundGenerationId(
                1), credits(0) {
    gto = 3 * generation_size;
    sgtt = -1;
    gttvar = -1;
    gto_alpha = 1.0 / 8.0;
    gto_beta = 1.0 / 4.0;
}

Flow::~Flow() {
    for_each(generations.begin(), generations.end(),
            [this](std::shared_ptr<Generation> generation) {
                mainNcorp->cancelEvent(generation->getTimeoutMsg());
            });
}
/*
 * Calcula o diferencia backlog de todas as gerações da janela de transmissão
 */
double Flow::calculateDifferentialBacklog() {
    double dbl = 0;
    for (auto gen = generations.begin();
            gen != generations.end()
                    && ((*gen)->getId() - leftBoundGenerationId) < window_size;
            gen++) {
        dbl += (*gen)->calculateDifferentialBacklog();
    }
    return dbl;
}

uint16_t Flow::getId() const {
    return id;
}

uint16_t Flow::nextGenerationId() {
    return generations.empty() ?
            leftBoundGenerationId : (*generations.rbegin())->getId() + 1;
}

//Recupera a geração corrente, dentro da janela de transmissão para transmissão ou um objeto falso (operator bool() )
std::shared_ptr<Generation> Flow::getCurrentGeneration() {
    for (auto i = generations.begin();
            i != generations.end()
                    && ((*i)->getId() - leftBoundGenerationId) < window_size;
            i++) {
        if ((*i)->isTransmitting())
            return *i;
    }
    return std::shared_ptr<Generation>();
}

bool Flow::isReadyToPushUp() {
    //Retorna verdadeiro se houver pacote a ser enviado para a camada de app
    return !rawDataOutput.empty();
}

void Flow::retrieveDecodedBlock(uint16_t& flowId,
        std::shared_ptr<std::vector<uint8_t> > vec) {
    flowId = id;
    std::copy(rawDataOutput.begin(), rawDataOutput.end(),
            back_inserter(*vec.get()));
    rawDataOutput.clear();
    //Recupera o bloco de dados originado do processo de decodificação
}

/**
 * Manipula um pacote codificado, retornando um pacote a ser enviado caso necessário.
 * Se nenhum pacote tiver de ser enviado, retorna NULL.
 *
 * O pacote codificador (segundo parâmetro) armazena ao mesmo tempo o payload e o vetor de codificação
 */
NcorpPacket* Flow::handleCodedPacket(uint16_t generationId, uint16_t baseWindow,
        std::shared_ptr<std::vector<uint8_t> > codedPacket,
        size_t payloadSize) {
    switch (role) {
    case SENDER: { //Não há o que se tratar no cliente
        return NULL;
    }
        break;
    case RELAY: {
//        3) Relay downstream
//        3.1) Recupera o id da geração e a janela base
//        3.2) Atualiza o valor de lb para o nova valor de janela base
//        3.3) Remove todas as gerações mais antigas que lb
//        3.4) Busca pela geração com o id passado por parâmetro (se não existir ainda a geração, cria)
//        3.5) Se a geração recuperada for igual a próxima geração a ser transmitida, sai silenciosamente. Caso contrário, transmite um pacote Coded-Ack
        if (leftBoundGenerationId < baseWindow)
            leftBoundGenerationId = baseWindow;

        if (generationId < leftBoundGenerationId) {
            auto packet = new CodedEAck();
            packet->setNextHopAddr(source);
//        packet->setSeqNum() //Defined at Ncorp level
            packet->setFlowId(id);
            packet->setGenerationId(leftBoundGenerationId);
            return packet;

        }

        std::for_each(generations.begin(), generations.end(),
                [this](std::shared_ptr<Generation> generation) {
                    if(this->leftBoundGenerationId > generation->getId()) {
                        this->generations.erase(generation);
                    }

                }); //Remove todas as gerações antigas

        auto generationIt = std::find_if(generations.begin(), generations.end(),
                [generationId](std::shared_ptr<Generation> generation) {
                    return generationId == generation->getId();
                });

        std::shared_ptr<Generation> generation;
        if (generationIt == generations.end()) { //A geração não existe
            std::shared_ptr<Generation> newGeneration(
                    new Generation(nodeId.getInt(), generationId, role, generation_size,
                            symbol_size, payloadSize));
            generations.insert(newGeneration);
            generation = newGeneration;
        } else {
            generation = *generationIt;
        }

        generation->pushEncodedData(codedPacket); //Repassa o pacote codificado para a geração correta

        if (generation == getCurrentGeneration()) {
            return NULL;
        } else {
            auto packet = new CodedAck();
            packet->setFlowId(id);
            packet->setAckGenerationId(generationId);
            packet->setAckVector(generation->retrieveCurrentAckVector());
            return packet;
        }

    }
        break;
    case RECEIVER: {
//            4) Receptor
//            4.1) Recupera o id da geração
//            4.2) Se for menor que o LB, retorna o eack mais atual
//            4.3) Busca pela geração com o id passado por parâmetro (se não existir ainda a geração, cria)
//            4.4) Repassa o pacote codificado para a geração recuperada
//            4.5) Se a geração recuperada for igual a lb e estiver completa, itera em todas as gerações já completas
//            4.5.1) decodifica toda a geração
//            4.5.2) repassa o bloco decodificado para a aplicação
//            4.5.3) remove a geração
//            4.5.4) incrementa o lb
//            4.5.3) responde com um e-ack
//            4.5.4) sai
//            4.6) gera o pacote coded-ack e sai
        if (generationId < leftBoundGenerationId) {
            auto packet = new CodedEAck();
            packet->setNextHopAddr(source); //This address will be replaced for the best neighbour
//        packet->setSeqNum() //Defined at Ncorp level
            packet->setFlowId(id);
            packet->setGenerationId(leftBoundGenerationId);
            return packet;
        }

        auto generationIt = std::find_if(generations.begin(), generations.end(),
                [generationId](std::shared_ptr<Generation> generation) {
                    return generationId == generation->getId();
                });

        std::shared_ptr<Generation> generation;
        if (generationIt == generations.end()) { //A geração não existe
            std::shared_ptr<Generation> newGeneration(
                    new Generation(nodeId.getInt(), generationId, role, generation_size,
                            symbol_size, payloadSize));
            generations.insert(newGeneration);
            generation = newGeneration;
        } else {
            generation = *generationIt;
        }

        generation->pushEncodedData(codedPacket);
        if (generation->getId() == leftBoundGenerationId
                && generation->isComplete()) {
            for (auto it = generations.begin();
                    it != generations.end()
                            && (*it)->getId() == leftBoundGenerationId
                            && (*it)->isComplete(); it++) {
                auto output = (*it)->getDecodedBlock(); //Retrieve the decoded block
                copy(output->begin(), output->end(),
                        back_inserter(rawDataOutput)); //Copy the vector to the output
                generations.erase(it);
                leftBoundGenerationId++;
            }
            auto packet = new CodedEAck();
            packet->setNextHopAddr(source);
//        packet->setSeqNum() //Defined at Ncorp level
            packet->setFlowId(id);
            packet->setGenerationId(leftBoundGenerationId);
            return packet;

        } else {
            auto packet = new CodedAck();
            packet->setFlowId(id);
            packet->setAckGenerationId(generation->getId());
            packet->setAckVector(generation->retrieveCurrentAckVector());
            return packet;
        }

    }
    default:
        return NULL;
    }
}

//Manipula um pacote eack. Todas as gerações menores que generationId são descartadas
void Flow::handleEAck(uint16_t generationId) {

    if (role != RECEIVER) {
        if (generationId < leftBoundGenerationId)
            return;

        if (role == SENDER)
            updateGto(generationId);

        leftBoundGenerationId = generationId;
        for (auto it = generations.begin();
                it != generations.end()
                        && (*it)->getId() < leftBoundGenerationId; it++) {
            mainNcorp->cancelEvent((*it)->getTimeoutMsg());
            generations.erase(it); //Remove todas as gerações anteriores ao LB
        }
    }
}

void Flow::updateGto(uint16_t generationId) {
    auto generationIt = std::find_if(generations.begin(), generations.end(),
            [generationId](std::shared_ptr<Generation> generation) {
                return generationId == generation->getId();
            });

    if (generationIt != generations.end()) {
        auto generation = *generationIt;
        auto msg = generation->getTimeoutMsg();
        auto currentTime = simTime();
        auto startMsgTime = msg->getSendingTime();
        simtime_t r = currentTime - startMsgTime;

        if (gttvar < 0) {
            gttvar = r / 2;
            sgtt = r;
        } else {
            gttvar = (1 - gto_beta) * gttvar + gto_beta * fabs(sgtt - r);
            sgtt = (1 - gto_alpha) * sgtt + gto_alpha * r;
        }

        gto = sgtt + max(simtime_t(1), 4 * gttvar);
    }
}

//
void Flow::pushRawBlock(std::vector<uint8_t> payload) {
    std::copy(payload.begin(), payload.end(), std::back_inserter(rawData));
    if (rawData.size() >= generation_size * symbol_size) //Verifica se foi ultrapassado o tamanho total de dados da geração
            { //Repassa os dados para a geração
        std::vector<uint8_t> newGen;
        std::copy_n(rawData.begin(), generation_size * symbol_size,
                std::back_inserter(newGen));
        rawData.erase(rawData.begin(),
                rawData.begin() + (generation_size * symbol_size));
        auto genId = nextGenerationId();
        std::shared_ptr<Generation> generation(
                new Generation(nodeId.getInt(), genId, generation_size, symbol_size,
                        newGen, generation_size * symbol_size));
        generations.insert(generation);
    }
}

void Flow::flush() {
    if (rawData.empty()) {
        return;
    }

    std::vector<uint8_t> newGen;
    std::copy(rawData.begin(), rawData.end(), std::back_inserter(newGen));
    rawData.clear(); //Limpa buffer

    auto validPayload = newGen.size();
    size_t remainingBufferSize = (generation_size * symbol_size) - validPayload;
    std::vector<uint8_t> padding(remainingBufferSize, 0);
    std::copy(padding.begin(), padding.end(), back_inserter(newGen));

    auto genId = nextGenerationId();
    std::shared_ptr<Generation> generation(
            new Generation(nodeId.getInt(), genId, generation_size, symbol_size, newGen,
                    validPayload));
    generations.insert(generation);
}

//
void Flow::handleAckCoding(uint16_t generationId,
        std::vector<uint8_t> ackCoding, uint32_t nodeId) {
    auto generation = std::find_if(generations.begin(), generations.end(),
            [generationId](std::shared_ptr<Generation> entry) {
                return entry->getId() == generationId;
            });

    if (generation != generations.end()) {
        (*generation)->pushAckCoding(ackCoding, nodeId);
    }
}
//
CodedDataAck* Flow::generateNewPacket() {

    switch (role) {
    case SENDER: {
        auto currentGeneration = getCurrentGeneration();
        if (!currentGeneration)
            return NULL;

        auto packet = new CodedDataAck();
        packet->setFlowSrcAddr(source);
        packet->setFlowDstAddr(destination);
//        packet->setSeqNum() //Defined at Ncorp level
        packet->setFlowId(id);
        packet->setBaseWindow(leftBoundGenerationId);
        packet->setGenerationId(currentGeneration->getId());
        packet->setUseAck(false); //Se for um relay terá ack
        //        packet->setce() //O tamanho do con
        //        packet->setDbl(); //O valor do dbl é definido um nível acima, em nível de CCACK
        //        packet->setForwardSet() //Definido em nível de ncorp
        packet->setPayloadSize(currentGeneration->getPayloadSize());

        auto p = currentGeneration->generateEncodedPacket();
        packet->setEncodingVector(p.first);
        packet->setPayload(p.second);

        auto timeout = currentGeneration->getTimeoutMsg();
        if (!timeout->isScheduled()) {
            timeout->setContextPointer(this);
            mainNcorp->scheduleTimer(gto, timeout);
        }

        return packet;
    }
        break;
    case RELAY: {
        auto currentGeneration = getCurrentGeneration();
        if (!currentGeneration)
            return NULL;

        auto packet = new CodedDataAck();
        packet->setFlowSrcAddr(source);
        packet->setFlowDstAddr(destination);
//        packet->setSeqNum() //Definido em nível de ncorp
        packet->setFlowId(id);
        packet->setBaseWindow((*generations.begin())->getId());
        packet->setGenerationId(currentGeneration->getId());
        packet->setUseAck(true); //Se for um relay terá ack
        //        packet->setce() //O tamanho do con
        //        packet->setDbl(); //O valor do dbl é definido um nível acima, em nível de CCACK
        //        packet->setForwardSet() //Definido em nível de ncorp

        packet->setAckVector(currentGeneration->retrieveCurrentAckVector());
        auto p = currentGeneration->generateEncodedPacket();
        packet->setEncodingVector(p.first);
        packet->setPayload(p.second);
        packet->setPayloadSize(currentGeneration->getPayloadSize());
        return packet;
    }
        break;
    default:
        return NULL;
    }
}

//
//Retorna o destino do fluxo
IPv4Address Flow::getDestination() const {
    return destination;
}
//
//Retorna o tipo de fluxo (origem, relay e destino)
Role Flow::getRole() const {
    return role;
}
//
//Retorna a origem do fluxo
IPv4Address Flow::getSource() const {
    return source;
}

double Flow::getCredits() const {
    return credits;
}
void Flow::setCredits(double credits) {
    this->credits = credits;
}

void Flow::processTimer(cMessage* msg) {
    switch (msg->getKind()) {
    case GENERATION_TIMEOUT: {
        auto generationIt = std::find_if(generations.begin(), generations.end(),
                [msg](std::shared_ptr<Generation> generation) {
                    return msg == generation->getTimeoutMsg();
                });

        if (generationIt != generations.end()) {
            leftBoundGenerationId = (*generationIt)->getId() + 1;

            for (auto it = generations.begin();
                    it != generations.end()
                            && (*it)->getId() < leftBoundGenerationId; it++) {
                mainNcorp->cancelEvent((*it)->getTimeoutMsg());
                generations.erase(it); //Remove todas as gerações anteriores ao LB
            }

        } else {
            cRuntimeError(mainNcorp,
                    "Erro inválido na sincronização de gerações!");
        }
    }
        break;
    default:
        delete msg;
        break;
    }
}

void Flow::printInfo() {

}

}

/* namespace ncorp */
