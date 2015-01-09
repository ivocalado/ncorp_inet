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
#include <functional>
#include <tuple>
#include "helper-functions.h"
using namespace std;
#include "Ncorp.h"

//
namespace ncorp {
//
//using namespace std;
//
Flow::Flow(Ncorp* mNcorp, IPv4Address nId, uint16_t ident, IPv4Address src,
        IPv4Address dest) :
        mainNcorp(mNcorp), nodeId(nId), id(ident), source(src), destination(
                dest), window_size(1), baseDT(MAXTIME), leftBoundGenerationId(1),credits(0), generations(
                [](const std::shared_ptr<Generation>& g1, const std::shared_ptr<Generation>& g2) {return g1->getId() < g2->getId();}) {
    generation_size = mainNcorp->par("CCACKGenerationSize");
    gto_alpha = mainNcorp->par("CCACKGtoAlpha"); //1.0 / 8.0;
    gto_beta = mainNcorp->par("CCACKGtoBeta"); //1.0 / 4.0;
    useSingleGeneration = mainNcorp->par("CCACKUseSingleGeneration");
    symbol_size = mainNcorp->par("CCACKPayloadSize");

    if(nodeId == source)
        role = SENDER;
    else if(nodeId == destination)
        role = RECEIVER;
    else role = RELAY;

    gto = 3 * generation_size;
    sgtt = -1;
    gttvar = -1;

    fprintf(stderr, "gto = %f\nsgtt = %f\ngttvar = %f\n\n", gto.dbl(),
            sgtt.dbl(), gttvar.dbl());
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
                    && ((*gen)->getId() - leftBoundGenerationId)
                            < calculateWindowSize(); gen++) {
        if((*gen)->isTransmitting()) {
            dbl = (*gen)->calculateDifferentialBacklog();
            break;
        }

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
    std::shared_ptr<Generation> result;

    if (role == SENDER) {
        for (auto i = generations.begin();
                i != generations.end()
                        && ((*i)->getId() - leftBoundGenerationId)
                                < calculateWindowSize(); i++) {
            if ((*i)->isTransmitting())
                result = *i;
        }
    } else if (role == RELAY) {
        for (auto i = generations.begin(); i != generations.end(); i++) {
            if ((*i)->isTransmitting())
                result = *i;
        }
    }

    return result;
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
    debugprintf(stderr, LOG_LEVEL_3, "Flow::handleCodedPacket Begin\n");
    NcorpPacket* result = NULL;
    switch (role) {
    case SENDER: {
        debugprintf(stderr, LOG_LEVEL_3, "Sender nao manipula pacotes\n");
    }
        break;
    case RELAY: {
        debugprintf(stderr, LOG_LEVEL_3, "RELAY\n");
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
            packet->setFlowSrcAddr(source);
//        packet->setSeqNum() //Defined at Ncorp level
            packet->setFlowId(id);
            packet->setGenerationId(leftBoundGenerationId);

            debugprintf(stderr, LOG_LEVEL_3,
                    "Pacote mais antigo que a geração atual. Retornando um CODEDEACK\n");
            result = packet;
            break;

        }

        std::vector<std::shared_ptr<Generation>> generationsToDrop;
        std::for_each(generations.begin(), generations.end(),
                [this, &generationsToDrop](std::shared_ptr<Generation> generation) {
                    if(this->leftBoundGenerationId > generation->getId()) {
                        debugprintf(stderr, LOG_LEVEL_3, "Removendo geração antiga\n");
                        generationsToDrop.push_back(generation);
//                        this->generations.erase(generation);
                    }

                }); //Remove todas as gerações antigas

        std::for_each(generationsToDrop.begin(), generationsToDrop.end(),
                [this](std::shared_ptr<Generation> gen) {
                    generations.erase(gen);
                });

        auto generationIt = std::find_if(generations.begin(), generations.end(),
                [generationId](std::shared_ptr<Generation> generation) {
                    return generationId == generation->getId();
                });

        std::shared_ptr<Generation> generation;
        if (generationIt == generations.end()) { //A geração não existe
            debugprintf(stderr, LOG_LEVEL_3, "Criando nova geracao\n");
            std::shared_ptr<Generation> newGeneration(
                    new Generation(nodeId.getInt(), generationId, role,
                            generation_size, symbol_size, payloadSize));
            generations.insert(newGeneration);
            generation = newGeneration;
        } else {
            generation = *generationIt;
        }

        debugprintf(stderr, LOG_LEVEL_3, "Repassando dados para a geracao\n");
        generation->pushEncodedData(codedPacket); //Repassa o pacote codificado para a geração correta

        if (generation != getCurrentGeneration()) {
            debugprintf(stderr, LOG_LEVEL_3,
                    "Retornando pacote CodedAck pois a geracao recebida eh diferente da corrente\n");
            auto packet = new CodedAck();
            packet->setFlowId(id);
            packet->setAckGenerationId(generationId);
            packet->setAckVector(generation->retrieveCurrentAckVector());
            result = packet;
        }

    }
        break;
    case RECEIVER: {
        debugprintf(stderr, LOG_LEVEL_3, "RECEIVER\n");
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
            packet->setFlowSrcAddr(source);
//        packet->setSeqNum() //Defined at Ncorp level
            packet->setFlowId(id);
            packet->setGenerationId(leftBoundGenerationId);
            result = packet;
            debugprintf(stderr, LOG_LEVEL_3,
                    "Pacote mais antigo que a geração atual. Retornando um CODEDEACK\n");
            break;
        }

        auto generationIt = std::find_if(generations.begin(), generations.end(),
                [generationId](std::shared_ptr<Generation> generation) {
                    return generationId == generation->getId();
                });

        std::shared_ptr<Generation> generation;
        if (generationIt == generations.end()) { //A geração não existe
            debugprintf(stderr, LOG_LEVEL_3, "Criando nova geracao\n");
            std::shared_ptr<Generation> newGeneration(
                    new Generation(nodeId.getInt(), generationId, role,
                            generation_size, symbol_size, payloadSize));
            generations.insert(newGeneration);
            generation = newGeneration;
        } else {
            generation = *generationIt;
        }

        generation->pushEncodedData(codedPacket);
        if (generation->getId() == leftBoundGenerationId
                && generation->isComplete()) {
            debugprintf(stderr, LOG_LEVEL_3,
                    "Geracao completa. Entregando dados da geracao\n");
            std::vector<std::shared_ptr<Generation>> generationsToDrop;
            for (auto it = generations.begin();
                    it != generations.end()
                            && (*it)->getId() == leftBoundGenerationId
                            && (*it)->isComplete(); it++) {
                auto output = (*it)->getDecodedBlock(); //Retrieve the decoded block
                copy(output->begin(), output->end(),
                        back_inserter(rawDataOutput)); //Copy the vector to the output
                generationsToDrop.push_back(*it);
                leftBoundGenerationId++;
            }

            std::for_each(generationsToDrop.begin(), generationsToDrop.end(),
                    [this](std::shared_ptr<Generation> g) {
                        generations.erase(g);
                    });

            debugprintf(stderr, LOG_LEVEL_3, "Retornando CODED_EACK\n");
            auto packet = new CodedEAck();
            packet->setFlowSrcAddr(source);
//        packet->setSeqNum() //Defined at Ncorp level
            packet->setFlowId(id);
            packet->setGenerationId(leftBoundGenerationId);
            result = packet;

        } else {
            debugprintf(stderr, LOG_LEVEL_3, "Retornando CODED_ACK\n");
            auto packet = new CodedAck();
            packet->setFlowId(id);
            packet->setAckGenerationId(generation->getId());
            packet->setAckVector(generation->retrieveCurrentAckVector());
            result = packet;
        }

    }
    default:
        break;
    }

    debugprintf(stderr, LOG_LEVEL_3, "Flow::handleCodedPacket End\n");
    return result;
}

uint16_t estimateNewWindowSize(uint16_t currentWindowSize,
        double expectedSendingRate, double actualSendingRate,
        simtime_t baseDT) {
    double d = expectedSendingRate - actualSendingRate;
    double alpha = 1.0 / baseDT.dbl();
    double beta = 3.0 / baseDT.dbl();

    uint16_t newWindowSize = 1;

    if (d < alpha)
        newWindowSize = currentWindowSize + 1;
    else if (alpha <= d && d <= beta)
        newWindowSize = currentWindowSize;
    else
        newWindowSize = currentWindowSize - 1;

    fprintf(stderr,
            "current_window_size = %d\nexpected_sending_rate = %f\nactual_sending_rate = %f\nd = %f\nbaseDT = %f\nalpha = %f\nbeta = %f\nnew_window_size = %d\n\n",
            currentWindowSize, expectedSendingRate, actualSendingRate, d,
            baseDT.dbl(), alpha, beta, newWindowSize);
    return newWindowSize;
}

//Manipula um pacote eack. Todas as gerações menores que generationId são descartadas
void Flow::handleEAck(uint16_t generationId) {

    if (role != RECEIVER) {
        if (generationId <= leftBoundGenerationId)
            return;

        if (role == SENDER) {
            updateGto(generationId); //

            std::for_each(generations.begin(), generations.end(),
                    [](std::shared_ptr<Generation> g) {
                        g->notifyGenerationReception();
                    });

        }

        leftBoundGenerationId = generationId;
        std::vector<std::shared_ptr<Generation>> generationsToDrop;

        for (auto it = generations.begin();
                it != generations.end()
                        && (*it)->getId() < leftBoundGenerationId; it++) {
            mainNcorp->cancelEvent((*it)->getTimeoutMsg());

            if (role == SENDER) {
                (*it)->stopTransmit();
                baseDT = std::min(baseDT, (*it)->calculateDT());
            }
            generationsToDrop.push_back(*it);
        }

        if (role == SENDER) {
            auto lastAckedGeneration = generationsToDrop.back();
            auto actualSendingRate =
                    lastAckedGeneration->calculateActualSendingRate();

            window_size = estimateNewWindowSize(window_size,
                    calculateExpectedSendingRate(), actualSendingRate, baseDT);
        }

        std::for_each(generationsToDrop.begin(), generationsToDrop.end(),
                [this](std::shared_ptr<Generation> g) {
                    generations.erase(g);
                });
    }

}

void Flow::updateGto(uint16_t generationId) {
    auto ackedGeneration = generationId - 1;
    auto generationIt = std::find_if(generations.begin(), generations.end(),
            [ackedGeneration](std::shared_ptr<Generation> generation) {
                return ackedGeneration == generation->getId();
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

        gto = max(simtime_t(1), sgtt + 4 * gttvar);

        fprintf(stderr, "R' = %f\ngto = %f\nsgtt = %f\ngttvar = %f\n\n",
                r.dbl(), gto.dbl(), sgtt.dbl(), gttvar.dbl());
    }
}

//
void Flow::pushRawBlock(std::vector<uint8_t> payload) {
    std::copy(payload.begin(), payload.end(), std::back_inserter(rawData));
    fprintf(stderr, "Bloco recebido. Size = %lu\n", payload.size());
    while (rawData.size() >= generation_size * symbol_size) //Verifica se foi ultrapassado o tamanho total de dados da geração
    { //Repassa os dados para a geração
        std::vector<uint8_t> newGen;
        std::copy_n(rawData.begin(), generation_size * symbol_size,
                std::back_inserter(newGen));
        rawData.erase(rawData.begin(),
                rawData.begin() + (generation_size * symbol_size));
        auto genId = nextGenerationId();
        std::shared_ptr<Generation> generation(
                new Generation(nodeId.getInt(), genId, generation_size,
                        symbol_size, newGen, generation_size * symbol_size));
        generations.insert(generation);
        fprintf(stderr, "Geração criada ==> Id = %d!\n", genId);
        fprintf(stderr, "rawDate::Size = %lu\n", rawData.size());
    }
}

void Flow::flush() {
    if (rawData.empty()) {
        return;
    }

    std::vector<uint8_t> newGen;
    std::copy(rawData.begin(), rawData.end(), std::back_inserter(newGen));
    rawData.clear(); //Limpa buffer
    fprintf(stderr, "newGen::Size = %lu\n", newGen.size());

    auto validPayload = newGen.size();
    size_t remainingBufferSize = (generation_size * symbol_size) - validPayload;
    std::vector<uint8_t> padding(remainingBufferSize, 0);
    std::copy(padding.begin(), padding.end(), back_inserter(newGen));

    auto genId = nextGenerationId();
    fprintf(stderr, "Geração criada ==> Id = %d\n", genId);
    std::shared_ptr<Generation> generation(
            new Generation(nodeId.getInt(), genId, generation_size, symbol_size,
                    newGen, validPayload));
    generations.insert(generation);
}

//
void Flow::handleAckCoding(uint16_t generationId,
        std::vector<uint8_t> ackCoding, uint32_t nodeId) {
    debugprintf(stderr, LOG_LEVEL_3, "Flow::handleAckCoding Begin\n");
    auto generation = std::find_if(generations.begin(), generations.end(),
            [generationId](std::shared_ptr<Generation> entry) {
                return entry->getId() == generationId;
            });

    if (generation != generations.end()) {
        (*generation)->pushAckCoding(ackCoding, nodeId);
    }
    debugprintf(stderr, LOG_LEVEL_3, "Flow::handleAckCoding End\n");
}
//
CodedDataAck* Flow::generateNewPacket() {
    debugprintf(stderr, LOG_LEVEL_3, "Flow::generateNewPacket Begin\n");

    CodedDataAck* result = NULL;

    switch (role) {
    case SENDER: {
        debugprintf(stderr, LOG_LEVEL_3, "SENDER\n");
        auto currentGeneration = getCurrentGeneration();
        if (!currentGeneration) {
            debugprintf(stderr, LOG_LEVEL_3, "Nenhuma geracao selecionada\n");
            break;
        }

        currentGeneration->startTransmit();

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
        packet->setEncodingVector(std::get < 0 > (p));
        packet->setPayload(std::get < 1 > (p));
        packet->setInovative(std::get < 2 > (p));

        auto timeout = currentGeneration->getTimeoutMsg();
        if (!timeout->isScheduled()) {
            timeout->setContextPointer(this);
            mainNcorp->scheduleTimer(gto, timeout);
        }

        result = packet;
    }
        break;
    case RELAY: {
        debugprintf(stderr, LOG_LEVEL_3, "RELAY\n");
        auto currentGeneration = getCurrentGeneration();
        if (!currentGeneration)
            break;

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
        packet->setEncodingVector(std::get < 0 > (p));
        packet->setPayload(std::get < 1 > (p));
        packet->setInovative(std::get < 2 > (p));
        packet->setPayloadSize(currentGeneration->getPayloadSize());
        result = packet;
    }
        break;
    default:
        break;
    }

    debugprintf(stderr, LOG_LEVEL_3, "Flow::generateNewPacket End\n");
    return result;
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

            std::vector<std::shared_ptr<Generation>> generationsToDrop;
            for (auto it = generations.begin();
                    it != generations.end()
                            && (*it)->getId() < leftBoundGenerationId; it++) {
                mainNcorp->cancelEvent((*it)->getTimeoutMsg());
                generationsToDrop.push_back(*it);
            }

            std::for_each(generationsToDrop.begin(), generationsToDrop.end(),
                    [this](std::shared_ptr<Generation> gen) {
                        generations.erase(gen);
                    });

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

uint16_t Flow::calculateWindowSize() {
    return useSingleGeneration ? 1 : window_size;
}

double Flow::calculateExpectedSendingRate() {
    return static_cast<double>(window_size) / baseDT.dbl();
}

}

/* namespace ncorp */
