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

using namespace std;

Define_Module(Ncorp)
;

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

        auto macModule =
                getParentModule()->getSubmodule("wlan", 0)->getSubmodule("mac",
                        0);

        appOutputGate = gate("wlanOut");
        appInputGate = gate("wlanIn");

        macModule->gate("appOut")->connectTo(appInputGate);

//        cerr<<"Conectado ao modulo com id: "<<appInputGate->getPathEndGate()->getOwnerModule()->getId()<<endl;

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

    EV<<"Host: "<<getMyNetAddr()<<" conectado na porta: "<<connectionPort<<endl;

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
    auto pkt = PK(pk);
    auto from =
    check_and_cast<UDPDataIndication *>(pkt->getControlInfo())->getSrcAddr().get4();

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
                handleEAckPkt(packet);
            }
                break;


            default: {
                throw new cRuntimeError(this, "Mensagem desconhecida!");
            }
        }
    askForChannelOpportunity();
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

        auto dst = IPvXAddressResolver().resolve(par("destAddress").stringValue()).get4();

        if (metric->findBestNeighbourTo(dst)
                == IPv4Address::UNSPECIFIED_ADDRESS) { //Nao ha rota
            throw cRuntimeError(this, "Não há rota para o destino %s",
                    dst.str(true).c_str());
        } else {
            ccackBaseline->pushRawBlock(getMyNetAddr(), dst, flowId, data_in);
        }
        fprintf(stderr, "%s <=> %s", getMyNetAddr().str(true).c_str(), dst.str(true).c_str());

        askForChannelOpportunity();
    }
        break;
    case GENERATION_TIMEOUT: {
//        fprintf(stderr, "\n[Node = %lu] [Time = %f] GENERATION_TIMEOUT\n", myNetwAddr, simTime().dbl());
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

void Ncorp::handleEAckPkt(CodedEAck* packet) {

    auto currentDestination = packet->getNextHopAddr();
    auto flowId = packet->getFlowId();
    auto generationId = packet->getGenerationId();

    auto destination = ccackBaseline->handleEAckPkt(packet);
    if (currentDestination == getMyNetAddr()
            && destination != IPv4Address::UNSPECIFIED_ADDRESS) {
        auto nextHop = metric->findBestNeighbourTo(destination);
        auto eackPacket = new CodedEAck();
        eackPacket->setNextHopAddr(nextHop);
        eackPacket->setFlowId(flowId);
        eackPacket->setGenerationId(generationId);
        ncorp::PacketSizeConfigurator().configure(eackPacket);

        sendToIp(eackPacket, IPv4Address::ALLONES_ADDRESS);
    } /*else {
     fprintf(stderr, "[Node = %lu] [Time = %f] MyNetworkLayer::handleEAckPkt -- > Interrompendo flood de EACK\n", myNetwAddr, simTime().dbl());
     }*/
}

void Ncorp::handleCodedAckPkt(CodedAck* packet, IPv4Address relay) {

    auto receivedFlowId = packet->getFlowId();
    if (!ccackBaseline->hasFlow(receivedFlowId)) {
        delete packet;
        return;
    }

    auto destination = ccackBaseline->getDestByFlow(receivedFlowId);
    if (relay == destination || isUpstream(relay, destination)) {
        ccackBaseline->handleCodedAckAtUpstream(packet, relay);
    } else
        delete packet;
}

simtime_t Ncorp::handleCodedDataAckPkt(CodedDataAck* packet,
        IPv4Address relay) {

    simtime_t delay = SIMTIME_ZERO;

    auto to = packet->getFlowDstAddr();
    auto fs = packet->getForwardSet();
    bool upstream = isUpstream(relay, to);

    NcorpPacket* result = NULL;

//    fprintf(stderr, "Myaddr: %s\n", getMyNetAddr().str(true).c_str());
//    std::for_each(fs.begin(), fs.end(), [](IPv4Address adr) {
//        fprintf(stderr, "%s\n", adr.str(true).c_str());
//    });
    if (to == getMyNetAddr()
            || std::find(fs.begin(), fs.end(), getMyNetAddr()) != fs.end()) {
        result = ccackBaseline->handleCodedDataAtDownstream(packet, relay,
                simTime());
        packet = NULL;
    } else if (upstream) {
        ccackBaseline->handleCodedDataAtUpstream(packet, relay, simTime());
    } else {
        delete packet;
        return delay;
    }

    if (result) {
        switch (result->getType()) {
        case CODED_EACK: {

            auto eackPkt = dynamic_cast<CodedEAck*>(result);

            ncorp::PacketSizeConfigurator().configure(eackPkt);
            auto realSource = eackPkt->getNextHopAddr();
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
            auto packet = dynamic_cast<CodedAck*>(result);
            ncorp::PacketSizeConfigurator().configure(packet);
            sendToIp(result, IPv4Address::ALLONES_ADDRESS);
        }
            break;
        default:
            break;
        }

    }
    return delay;
}

void Ncorp::deliverReceivedBlock(uint16_t flowId, IPv4Address from,
        std::shared_ptr<std::vector<uint8_t> > block) {
    cerr<<"Time = "<<simTime()<<" Received block. Size = "<<block->size()<<" from: "<<from<<" flowId = "<<flowId<<endl;
}

void Ncorp::handleMacControlMsg(cMessage* msg) {
        if(msg->getKind() == Ieee80211Mac::AVAILABLE_CHANNEL) {
            delete msg;
            auto packet_to_send = ccackBaseline->nextPacketToTransmit();
            if(packet_to_send) {

                sendToIp(packet_to_send, IPv4Address::ALLONES_ADDRESS);
            }
        } else {
            delete msg;
            opp_error("Mensagem invalida. Este código está correto?!?");
        }

//        case Mac80211::AVAILABLE_CHANNEL: {
//        fprintf(stderr, "\n[Node = %lu] [Time = %f]  MyNetworkLayer::handleLowerControl -- Begin\n", myNetwAddr, simTime().dbl());
//
//        fprintf(stderr, "[Node = %lu] [Time = %f]  MyNetworkLayer::handleLowerControl -- > Channel is available for transmission\n", myNetwAddr, simTime().dbl());
//        delete msg;
//        //fprintf(stderr, "[Node = %lu] [Time = %f] Preparando para enviar novo pacote -- Begin\n", myNetwAddr, simTime().dbl());
//        NetwPkt* packet_to_send = nextPacket();
//
//        fprintf(stderr, "[Node = %lu] [Time = %f]  MyNetworkLayer::handleLowerControl -- > Selecting packet for transmission\n", myNetwAddr, simTime().dbl());
//        if (packet_to_send) {
//            fprintf(stderr, "[Node = %lu] [Time = %f]  MyNetworkLayer::handleLowerControl -- > Ha pacote disponivel para transmissao\n", myNetwAddr, simTime().dbl());
//            sendDown(packet_to_send);
//        } else {
//            fprintf(stderr, "[Node = %lu] [Time = %f]  MyNetworkLayer::handleLowerControl -- > Nao ha pacote disponível para transmissao\n", myNetwAddr, simTime().dbl());
//        }
//        //fprintf(stderr, "[Node = %lu] [Time = %f] Preparando para enviar novo pacote -- End\n", myNetwAddr, simTime().dbl());
//        fprintf(stderr, "[Node = %lu] [Time = %f]  MyNetworkLayer::handleLowerControl -- End\n", myNetwAddr, simTime().dbl());
//
//    }
//        break;

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
