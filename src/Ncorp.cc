//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2007 Universidad de Málaga
// Copyright (C) 2011 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include "Ncorp.h"
#include "packets_m.h"
#include "EventHandler.h"

#include "UDPControlInfo_m.h"
#include "IPvXAddressResolver.h"
#include "InterfaceTableAccess.h"

#include "IRoutingTable.h"
#include "RoutingTableAccess.h"
#include "MobilityAccess.h"
#include "IPv4InterfaceData.h"
#include "ETXMetric.h"
#include "Flow.h"
#include <iostream>
#include "PacketSizeConfigurator.h"
#include "Ieee80211Mac.h"
#include "constants.h"
#include "helper-functions.h"

using namespace std;

Define_Module(Ncorp)
;

NcorpPacket* extractPkt(Ieee80211DataOrMgmtFrame* pkt) {
    auto dataFrame = dynamic_cast<Ieee80211DataFrame*>(pkt);

    if (dataFrame) {
        return dynamic_cast<NcorpPacket*>(dataFrame->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket());
    } else {
        return NULL;
    }

}

Ncorp::Ncorp() {
    outputInterface = -1;
    metric = NULL;
    appOutputGate = NULL;
    appInputGate = NULL;
}

Ncorp::~Ncorp() {
    if (metric)
        delete metric;
}

void Ncorp::initialize(int stage) {
    AppBase::initialize(stage);

    if (stage == 0) {
        connectionPort = par("connectionPort");

        macModule =
                (Ieee80211Mac*) getParentModule()->getSubmodule("wlan", 0)->getSubmodule(
                        "mac", 0);

        appOutputGate = gate("wlanOut");
        appInputGate = gate("wlanIn");

        macModule->gate("appOut")->connectTo(appInputGate);

        appOutputGate->connectTo(macModule->gate("appIn"));

    }
}

void Ncorp::handleMessageWhenUp(cMessage *msg) {
    if (msg->isSelfMessage())
        handleSelfMsg(msg);
    else
        handleLowerMsg(msg);
}

void Ncorp::finish() {
    metric->finish();
    AppBase::finish();
}

bool Ncorp::startApp(IDoneCallback *doneCallback) {

    socket.setOutputGate(gate("udpOut"));
    socket.bind(connectionPort);

    socket.setBroadcast(true);

    if (strcmp(par("outputInterface").stringValue(), "") != 0) {
        IInterfaceTable* ift = InterfaceTableAccess().get();
        InterfaceEntry *ie = ift->getInterfaceByName(
                par("outputInterface").stringValue());
        if (ie == NULL)
            throw cRuntimeError(this, "Invalid output interface name : %s",
                    par("outputInterface").stringValue());
        outputInterface = ie->getInterfaceId();
    } else {
        throw cRuntimeError(this, "Uma interface válida DEVE ser informada");
    }

    debugprintf(stderr, LOG_LEVEL_1, "Host: %s (%s) conectado na porta: %d\n",
            getMyNetAddr().str(true).c_str(), getMyNodeName().c_str(),
            connectionPort);
    metric = new ETXMetric;
    metric->setMainModule(this);
    ccackBaseline.reset(new ncorp::CcackBaseline(this));

    simtime_t sendTime = par("sendTime");

    if (par("destAddress").str() != "" && sendTime >= SIMTIME_ZERO) {
        cMessage* msg = new cMessage("message-data", NCORP_I_DATA);
        scheduleAt(sendTime, msg);
    }

    metric->initialize();

    return true;
}

bool Ncorp::stopApp(IDoneCallback *doneCallback) {

    //TODO if(socket.isOpened()) socket.close();
    return true;
}

bool Ncorp::crashApp(IDoneCallback *doneCallback) {
    return true;
}

IPv4Address Ncorp::getMyNetAddr() const {
    return InterfaceTableAccess().get()->getInterfaceById(outputInterface)->ipv4Data()->getIPAddress();
}

Coord Ncorp::getCurrentPosition() {
    return MobilityAccess().get()->getCurrentPosition();
}

double Ncorp::getTransmissionTime() {
    return metric->getTransmittionTime();
}

void Ncorp::scheduleTimer(simtime_t when, cMessage* msg) {
    scheduleAt(simTime() + when, msg);
}

void Ncorp::sendToIp(cPacket *pk, IPv4Address destAddr) {
    pk->setTimestamp();
    socket.sendTo(pk, destAddr, connectionPort, outputInterface);
}

void Ncorp::handleLowerMsg(cMessage *pk) {
    debugprintf(stderr, LOG_LEVEL_1, "(%s) Ncorp::handleLowerMsg Begin\n",
            getMyNodeName().c_str());
    auto pkt = PK(pk);
    auto from =
            check_and_cast<UDPDataIndication *>(pkt->getControlInfo())->getSrcAddr().get4();

    auto host = IPvXAddressResolver().findHostWithAddress(from);

    debugprintf(stderr, LOG_LEVEL_1, "Received from %s[%d]\n", host->getName(),
            host->getIndex());
    auto ncorpPkt = check_and_cast<NcorpPacket*>(pkt);
    switch (ncorpPkt->getType()) {
    // Just to make sure it is the beacon message we sent
    case ETX_PROBE: {
        metric->processNetPck(from, pkt);
        return;
    }

    case CODED_DATA_ACK: {
        CodedDataAck* packet = check_and_cast<CodedDataAck*>(ncorpPkt);
        handleCodedDataAckPkt(packet, from);
    }
        break;
    case CODED_ACK: {
        CodedAck* packet = check_and_cast<CodedAck*>(ncorpPkt);
        handleCodedAckPkt(packet, from);
    }
        break;
    case CODED_EACK: {
        auto packet = check_and_cast<CodedEAck*>(ncorpPkt);
        handleEAckPkt(packet, from);
    }
        break;

    default: {
        throw new cRuntimeError(this, "Mensagem desconhecida!");
    }
    }
    askForChannelOpportunity();
    debugprintf(stderr, LOG_LEVEL_1, "(%s) Ncorp::handleLowerMsg End\n\n",
            getMyNodeName().c_str());
}

void Ncorp::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
    case NCORP_I_DATA: {
        delete msg;

        auto flowId = par("flowId").longValue();
        while (flowId <= 0)
            flowId = intrand(INT_MAX);

        auto messageLength = par("messageLength").longValue();
        if (messageLength <= 0)
            throw cRuntimeError(this, "Tamanho inválido para pacote: %d",
                    messageLength);
        std::vector<uint8_t> data_in(messageLength);

        for (auto &e : data_in)
            e = rand() % 256;

        auto dst = IPvXAddressResolver().resolve(
                par("destAddress").stringValue()).get4();

        if (metric->findBestNeighbourTo(dst)
                == IPv4Address::UNSPECIFIED_ADDRESS) { //Nao ha rota
            throw cRuntimeError(this, "Não há rota para o destino %s",
                    dst.str(true).c_str());
        } else {
            ccackBaseline->pushRawBlock(getMyNetAddr(), dst, flowId, data_in);
        }
        debugprintf(stderr, LOG_LEVEL_1, "%s <=> %s\n",
                getMyNetAddr().str(true).c_str(), dst.str(true).c_str());
        askForChannelOpportunity();
    }
        break;
    case GENERATION_TIMEOUT: {
//        debugprintf(stderr, "\n[Node = %lu] [Time = %f] GENERATION_TIMEOUT\n", myNetwAddr, simTime().dbl());
        auto flow = (ncorp::Flow*) msg->getContextPointer();
        flow->processTimer(msg);
    }
        break;
    default:
        ((EventHandler*) msg->getContextPointer())->processTimer(msg);
    }
}
//myself => relay => target
bool Ncorp::isUpstream(IPv4Address relay, IPv4Address target) {
    auto fs = metric->findCandidateSet(target);
    return std::find(fs.begin(), fs.end(), relay) != fs.end();
}

void Ncorp::handleEAckPkt(CodedEAck* packet, IPv4Address from) {
    fprintf(stderr, "(%s)(%s) Ncorp::handleEAckPkt Begin\n",
            getMyNodeName().c_str(), convertToSrt(getMyNetAddr()).c_str());
    fprintf(stderr, "From = %s\n", convertToSrt(from).c_str());
    fprintf(stderr, "To = %s\n",
            convertToSrt(packet->getFlowSrcAddr()).c_str());
    debugprintf(stderr, LOG_LEVEL_1, "(%s) Ncorp::handleEAckPkt Begin\n",
            getMyNodeName().c_str());
    auto currentDestination = packet->getNextHopAddr();
    auto flowId = packet->getFlowId();
    auto generationId = packet->getGenerationId();

#if DROP_MAC_PKT
    auto transmissionQueue = macModule->transmissionQueue();

    debugprintf(stderr, LOG_LEVEL_1, "(%s) Iniciando remocao de pacotes antigos\n",
            getMyNodeName().c_str());

    transmissionQueue->remove_if(
            [flowId, generationId, this](Ieee80211DataOrMgmtFrame* frame) {
                if(!frame->isTransmitting()) {
                    auto appPkt = extractPkt(frame);
                    if(appPkt) {

                        switch (appPkt->getType()) {
                            case CODED_ACK: {
                                auto codedAck = check_and_cast<CodedAck*>(appPkt);
                                if(codedAck->getFlowId() == flowId && codedAck->getAckGenerationId() < generationId) {
                                    debugprintf(stderr, LOG_LEVEL_1, "(%s) Deletando pacote CODED_ACK\n", getMyNodeName().c_str());
                                    delete frame;
                                    return true;
                                }
                            }
                            break;
                            case CODED_DATA_ACK: {
                                auto codedDataAck = check_and_cast<CodedDataAck*>(appPkt);
                                if(codedDataAck->getFlowId() == flowId && codedDataAck->getGenerationId() < generationId) {
                                    debugprintf(stderr, LOG_LEVEL_1, "(%s) Deletando pacote CODED_DATA_ACK\n", getMyNodeName().c_str());

                                    delete frame;
                                    return true;
                                }
                            }
                            break;
                            case CODED_EACK: {
                                auto eack = check_and_cast<CodedEAck*>(appPkt);
                                if(eack->getFlowId() == flowId && eack->getGenerationId() <= generationId) {
                                    debugprintf(stderr, LOG_LEVEL_1, "(%s) Deletando pacote CODED_EACK\n", getMyNodeName().c_str());
                                    delete frame;
                                    return true;
                                }
                            }
                            default:
                            break;
                        }
                    }
                } else {
                    debugprintf(stderr, LOG_LEVEL_1, "(%s) Pacote atual esta sendo transmitido\n", getMyNodeName().c_str());
                }
                return false;
            });

#endif

    auto destination = ccackBaseline->handleEAckPkt(packet);
    if (currentDestination == getMyNetAddr()
            && destination != IPv4Address::UNSPECIFIED_ADDRESS) {
        fprintf(stderr, "(%s) Reenviando pacote EACK\n",
                getMyNodeName().c_str());
        auto nextHop = metric->findBestNeighbourTo(destination);
        auto eackPacket = new CodedEAck();
        eackPacket->setFlowSrcAddr(destination);
        eackPacket->setNextHopAddr(nextHop);
        eackPacket->setFlowId(flowId);
        eackPacket->setGenerationId(generationId);
        ncorp::PacketSizeConfigurator().configure(eackPacket);

        sendToIp(eackPacket, IPv4Address::ALLONES_ADDRESS);
    } else {
        fprintf(stderr, "(%s) Interrompendo o fluxo\n",
                getMyNodeName().c_str());
    }
    fprintf(stderr, "(%s) Ncorp::handleEAckPkt End\n", getMyNodeName().c_str());
}

void Ncorp::handleCodedAckPkt(CodedAck* packet, IPv4Address relay) {
    debugprintf(stderr, LOG_LEVEL_1, "(%s) handleCodedAckPkt Begin\n",
            getMyNodeName().c_str());
    auto flowId = packet->getFlowId();

    if (!ccackBaseline->hasFlow(flowId)) {
        delete packet;
        debugprintf(stderr, LOG_LEVEL_1, "(%s) O fluxo nao esta presente\n",
                getMyNodeName().c_str());
        debugprintf(stderr, LOG_LEVEL_1, "(%s) handleCodedAckPkt End\n",
                getMyNodeName().c_str());
        return;
    }

    auto destination = ccackBaseline->getDestByFlow(flowId);
    if (relay == destination || isUpstream(relay, destination)) {
        debugprintf(stderr, LOG_LEVEL_1, "(%s) Repassando para o ccack\n",
                getMyNodeName().c_str());
        auto generationId = packet->getAckGenerationId();

        ccackBaseline->handleCodedAckAtUpstream(packet, relay);

#if DROP_MAC_PKT
        debugprintf(stderr, LOG_LEVEL_1, "(%s) Iniciando remocao de pacotes antigos\n",
                getMyNodeName().c_str());
        auto transmissionQueue = macModule->transmissionQueue();
        transmissionQueue->remove_if(
                [flowId, generationId, this](Ieee80211DataOrMgmtFrame* frame) {
                    if(!frame->isTransmitting()) {
                        auto appPkt = dynamic_cast<CodedDataAck*>(extractPkt(frame));
                        if(appPkt) {
                            if(appPkt->getFlowId() == flowId && appPkt->getGenerationId() == generationId) {
                                debugprintf(stderr, LOG_LEVEL_1, "(%s) deletando pacote antigo\n", getMyNodeName().c_str());
                                delete frame;
                                return true;
                            }
                        }
                    }
                    return false;
                });

        auto packetToSend = ccackBaseline->replacePacket(flowId);
        if (packetToSend) {
            debugprintf(stderr, LOG_LEVEL_1, "(%s) Substituindo por novo pacote\n",
                    getMyNodeName().c_str());
            sendToIp(packetToSend, IPv4Address::ALLONES_ADDRESS);
        } else {
            debugprintf(stderr, LOG_LEVEL_1, "(%s) Nao ha pacote para substituir\n",
                    getMyNodeName().c_str());
        }
#endif

    } else {
        debugprintf(stderr, LOG_LEVEL_1,
                "(%s) O no atual eh um downstream. Nao ha o que fazer com o pacote\n",
                getMyNodeName().c_str());
        delete packet;
    }
    debugprintf(stderr, LOG_LEVEL_1, "(%s) handleCodedAckPkt End\n",
            getMyNodeName().c_str());
}

simtime_t Ncorp::handleCodedDataAckPkt(CodedDataAck* packet,
        IPv4Address relay) {
    debugprintf(stderr, LOG_LEVEL_1,
            "(%s) Ncorp::handleCodedDataAckPkt Begin\n",
            getMyNodeName().c_str());
    simtime_t delay = SIMTIME_ZERO;

    auto to = packet->getFlowDstAddr();
    auto fs = packet->getForwardSet();
    bool upstream = isUpstream(relay, to);
    auto flowId = packet->getFlowId();

    NcorpPacket* result = NULL;

    if (to == getMyNetAddr()
            || std::find(fs.begin(), fs.end(), getMyNetAddr()) != fs.end()) {
        debugprintf(stderr, LOG_LEVEL_1,
                "(%s) Invocando handleCodedDataAtDownstream\n",
                getMyNodeName().c_str());
        result = ccackBaseline->handleCodedDataAtDownstream(packet, relay,
                simTime());
        packet = NULL;
    } else if (upstream) {

        debugprintf(stderr, LOG_LEVEL_1,
                "(%s) Invocando handleCodedDataAtUpstream\n",
                getMyNodeName().c_str());

        auto generationId = packet->getGenerationId();

        ccackBaseline->handleCodedDataAtUpstream(packet, relay, simTime());

#if DROP_MAC_PKT

        auto transmissionQueue = macModule->transmissionQueue();

        debugprintf(stderr, LOG_LEVEL_1, "(%s) Iniciando processo de delecao de pacotes\n",
                getMyNodeName().c_str());

        transmissionQueue->remove_if(
                [flowId, generationId, this](Ieee80211DataOrMgmtFrame* frame) {
                    if(!frame->isTransmitting()) {
                        auto appPkt = dynamic_cast<CodedDataAck*>(extractPkt(frame));
                        if(appPkt) {
                            if(appPkt->getFlowId() == flowId && appPkt->getGenerationId() == generationId) {
                                debugprintf(stderr, LOG_LEVEL_1, "(%s) Pacote CodedDataAck deletado\n", getMyNodeName().c_str());
                                delete frame;
                                return true;
                            }
                        }
                    } else {
                        debugprintf(stderr, LOG_LEVEL_1, "(%s) Nao eh possivel deletar pacote pois esta sendo transmitido\n", getMyNodeName().c_str());
                    }
                    return false;
                });

        auto packetToSend = ccackBaseline->replacePacket(flowId);
        if (packetToSend) {
            debugprintf(stderr, LOG_LEVEL_1, "%s: substituindo pacote\n",
                    getMyNodeName().c_str());
            sendToIp(packetToSend, IPv4Address::ALLONES_ADDRESS);
        } else {
            debugprintf(stderr, LOG_LEVEL_1, "%s: Não ha pacote para substituir!\n",
                    getMyNodeName().c_str());
        }
#endif
    } else {
        debugprintf(stderr, LOG_LEVEL_1,
                "(%s) Nao eh nem upstream nem downstream. SAINDO!\n",
                getMyNodeName().c_str());
        delete packet;
    }

    if (result) {
        switch (result->getType()) {
        case CODED_EACK: {
            fprintf(stderr, "(%s) Retornando pacote CodedEACK\n",
                    getMyNodeName().c_str());
            fprintf(stderr, "FlowID = %d\n", flowId);
            auto eackPkt = dynamic_cast<CodedEAck*>(result);

            ncorp::PacketSizeConfigurator().configure(eackPkt);
            auto realSource = eackPkt->getFlowSrcAddr();

            auto bestNeighbour = metric->findBestNeighbourTo(realSource);
            eackPkt->setNextHopAddr(bestNeighbour);

            sendToIp(eackPkt, IPv4Address::ALLONES_ADDRESS);

            //Send uncoded data to application
            uint16_t flowId;
            std::shared_ptr<std::vector<uint8_t> > dataBlock(
                    new std::vector<uint8_t>);
            ccackBaseline->retrieveDecodedBlock(flowId, dataBlock);

            deliverReceivedBlock(flowId, realSource, dataBlock);

        }
            break;

        case CODED_ACK: {
            debugprintf(stderr, LOG_LEVEL_1,
                    "(%s) Retornando pacote CodedACK\n",
                    getMyNodeName().c_str());
            auto packet = dynamic_cast<CodedAck*>(result);
            ncorp::PacketSizeConfigurator().configure(packet);
            sendToIp(result, IPv4Address::ALLONES_ADDRESS);
        }
            break;
        default:
            break;
        }

    }
    debugprintf(stderr, LOG_LEVEL_1, "(%s) Ncorp::handleCodedDataAckPkt End\n",
            getMyNodeName().c_str());
    return delay;
}

void Ncorp::deliverReceivedBlock(uint16_t flowId, IPv4Address from,
        std::shared_ptr<std::vector<uint8_t> > block) {
    if (block->size())
        fprintf(stderr,
                "Time = %f Received block. Size = %lu from: %s flowId = %d\n",
                simTime().dbl(), block->size(), convertToSrt(from).c_str(),
                flowId);
}

void Ncorp::handleMacControlMsg(cMessage* msg) {
    if (msg->getKind() == Ieee80211Mac::AVAILABLE_CHANNEL) {
        debugprintf(stderr, LOG_LEVEL_1,
                "(%s) Ncorp::handleMacControlMsg Begin\n",
                getMyNodeName().c_str());
        delete msg;
        auto packet_to_send = ccackBaseline->nextPacketToTransmit();
        if (packet_to_send) {
            debugprintf(stderr, LOG_LEVEL_1, "(%s) Ha pacote para transmitir\n",
                    getMyNodeName().c_str());
            if (!packet_to_send->isInovative()) {
                debugprintf(stderr, LOG_LEVEL_1,
                        "(%s) Pacote nao inovativo. Verificar se já ha pacote na pilha de transmissao\n",
                        getMyNodeName().c_str());
                auto transmissionQueue = macModule->transmissionQueue();
                debugprintf(stderr, LOG_LEVEL_1, "Passou 1\n");
                for (auto it = transmissionQueue->begin();
                        it != transmissionQueue->end(); it++) {

                    debugprintf(stderr, LOG_LEVEL_1,
                            "Tentando descartar pacote\n");

                    debugprintf(stderr, LOG_LEVEL_1, "Passou 1.1\n");
                    auto oldPkt = dynamic_cast<CodedDataAck*>(extractPkt(*it));

                    if (oldPkt) {
                        debugprintf(stderr, LOG_LEVEL_1, "Passou 1.2\n");
                        if (oldPkt->getFlowId() == packet_to_send->getFlowId()
                                && oldPkt->getGenerationId()
                                        == packet_to_send->getGenerationId()) {
                            debugprintf(stderr, LOG_LEVEL_1,
                                    "(%s) Descartando pacote novo\n",
                                    getMyNodeName().c_str());
                            delete packet_to_send; //Nao eh necessario transmitir esse pacote
                            packet_to_send = NULL;
                        }
                    } else {
                        debugprintf(stderr, LOG_LEVEL_1, "Passou 1.3\n");
                        debugprintf(stderr, LOG_LEVEL_1,
                                "(%s) Tentando descartar pacote\n",
                                getMyNodeName().c_str());
                    }
                }
            }

        }

        if (packet_to_send) {
            sendToIp(packet_to_send, IPv4Address::ALLONES_ADDRESS);
        } else {
            debugprintf(stderr, LOG_LEVEL_1, "(%s) Nao ha pacote para enviar\n",
                    getMyNodeName().c_str());
        }

        debugprintf(stderr, LOG_LEVEL_1,
                "(%s) Ncorp::handleMacControlMsg End\n\n",
                getMyNodeName().c_str());
    } else {
        delete msg;
        opp_error("Mensagem invalida. Este código está correto?!?");
    }
}

void Ncorp::sendControlMsgToMac(cMessage* msg) {
    if (appOutputGate->isConnected())
        send(msg, appOutputGate);
    else {
        EV<<"appOutputGate gate is not connected on Ncorp"<<endl;
        delete msg;
    }
}

void Ncorp::handleMessage(cMessage *msg) {
    if (msg->getArrivalGate() == appInputGate)
        handleMacControlMsg(msg);
    else
        AppBase::handleMessage(msg);
}

void Ncorp::askForChannelOpportunity() {
    cMessage* msg = new cMessage("available-channel",
            Ieee80211Mac::AVAILABLE_CHANNEL);
    sendControlMsgToMac(msg);
}

std::list<IPv4Address> Ncorp::findCandidateSet(IPv4Address target) {
    return metric->findCandidateSet(target);
}

string Ncorp::getMyNodeName() {
    stringstream s;
    s << getParentModule()->getName() << "[" << getParentModule()->getIndex()
            << "]";
    return s.str();
}
