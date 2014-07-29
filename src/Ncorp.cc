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

Define_Module(Ncorp)
;

Ncorp::Ncorp() {
    outputInterface = -1;
    metric = NULL;
}

Ncorp::~Ncorp() {
    if(metric)
        delete metric;
}

void Ncorp::initialize(int stage) {
    AppBase::initialize(stage);

    if (stage == 0) {
        connectionPort = par("connectionPort");

        metric = new ETXMetric;
        metric->setMainModule(this);
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
        throw cRuntimeError(this, "A valid output interface MUST be informed");
    }

    EV<<"Host: "<<getMyNetAddr()<<" conectado na porta: "<<connectionPort<<endl;

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

void Ncorp::scheduleTimer(simtime_t when, cMessage* msg) {
    scheduleAt(simTime() + when, msg);
}

void Ncorp::sendToIp(cPacket *pk, IPv4Address destAddr) {
    pk->setTimestamp();
    socket.sendTo(pk, destAddr, connectionPort, outputInterface);
}

#include <iostream>
using namespace std;

void Ncorp::handleLowerMsg(cMessage *pk) {
    auto pkt = PK(pk);
    auto from =
            check_and_cast<UDPDataIndication *>(pkt->getControlInfo())->getSrcAddr();

    auto ncorpPkt = check_and_cast<NcorpPacket*>(pkt);
    switch (ncorpPkt->getType()) {
    // Just to make sure it is the beacon message we sent
    case ETX_PROBE: {
        metric->processNetPck(from.get4(), pkt);
        return;
    }
    default: {
        throw new cRuntimeError(this, "Mensagem desconhecida!");
    }
    }
}

void Ncorp::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
    case GENERATION_TIMEOUT: {
//        fprintf(stderr, "\n[Node = %lu] [Time = %f] GENERATION_TIMEOUT\n", myNetwAddr, simTime().dbl());
//        auto flow = (ncorp::Flow*) msg->getContextPointer();
//        flow->processTimer(msg);
    }
        break;
    default:
        ((EventHandler*) msg->getContextPointer())->processTimer(msg);
    }
}
