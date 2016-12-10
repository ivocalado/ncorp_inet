//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <NcorpTransportLayerHook.h>

Define_Module(NcorpTransportLayerHook);
NcorpTransportLayerHook::NcorpTransportLayerHook() {
    // TODO Auto-generated constructor stub

}

NcorpTransportLayerHook::~NcorpTransportLayerHook() {
    // TODO Auto-generated destructor stub
}
#include <iostream>
using namespace std;

/** @brief initialization function */
void NcorpTransportLayerHook::initialize(int stage) {
    cerr<<"inicializando o hook! :)"<<endl;
}


/** @brief handles received messages - forwards them to the routing module */
void NcorpTransportLayerHook::handleMessage(cMessage *msg) {
    if(msg->isSelfMessage())
        handleSelfMessage(msg);
    else if(msg->arrivedOn("upIn"))
        handleMessageFromHL(msg, msg->getArrivalGate()->getIndex());
    else handleMessageFromLL(msg, msg->getArrivalGate()->getIndex());
}

void NcorpTransportLayerHook::handleSelfMessage(cMessage *msg) {

}

void NcorpTransportLayerHook::handleMessageFromHL(cMessage *msg, int gateIndex) {
    cerr<<"NcorpTransportLayerHook::handleMessageFromHL :)"<<endl;
    if (dynamic_cast<RegisterTransportProtocolCommand *>(msg)) {
        cerr<<"Registrando protocolo!! :)"<<endl;
        RegisterTransportProtocolCommand *command = check_and_cast<RegisterTransportProtocolCommand *>(msg);
        mapping.addProtocolMapping(command->getProtocol(), msg->getArrivalGate()->getIndex());
        sendDown(msg, gateIndex);
    } else {
        cerr<<"Enviando para baixo! :)"<<endl;
        sendDown(msg, gateIndex);
    }
}

void NcorpTransportLayerHook::handleMessageFromLL(cMessage *msg, int gateIndex) {
    cerr<<"NcorpTransportLayerHook::handleMessageFromLL :)"<<endl;
    cerr<<"Enviando para cima! :)"<<endl;
    sendUp(msg, gateIndex);

}

void NcorpTransportLayerHook::sendUp(cMessage* msg, int gateIndex) {
    send(msg, gate("upOut", gateIndex));
}

void NcorpTransportLayerHook::sendDown(cMessage* msg, int gateIndex) {
    send(msg, gate("downOut", gateIndex));
}

