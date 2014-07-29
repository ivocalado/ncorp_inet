/*
 * EventHandler.cc
 *
 *  Created on: Jun 3, 2014
 *      Author: ivocalado
 */

#include "EventHandler.h"
#include "Ncorp.h"

EventHandler::EventHandler() {
    mainModule = NULL;
}

EventHandler::~EventHandler() {
    // TODO Auto-generated destructor stub
}

void EventHandler::setMainModule(Ncorp* mModule) {
    mainModule = mModule;
}

void EventHandler::scheduleTimer(simtime_t when, cMessage* msg) {
    ASSERT(mainModule != NULL);
    msg->setContextPointer(this);
    mainModule->scheduleTimer(when, msg);
}

void EventHandler::sendDown(cPacket* pkt, IPv4Address dest) {
    ASSERT(mainModule != NULL);
    mainModule->sendToIp(pkt, dest);
}

cPar& EventHandler::par(const char *parname) {
    ASSERT(mainModule != NULL);
    return mainModule->par(parname);
}

cMessage* EventHandler::cancelEvent(cMessage* msg) {
    ASSERT(mainModule != NULL);
    return mainModule->cancelEvent(msg);
}

IPv4Address EventHandler::getMyNetAddr() const {
    ASSERT(mainModule != NULL);
    return mainModule->getMyNetAddr();
}

Coord EventHandler::getMyPosition() const {
    ASSERT(mainModule != NULL);
    auto position = mainModule->getCurrentPosition();
    return position;
}

simsignal_t EventHandler::registerSignal(const char* name) {
    ASSERT(mainModule != NULL);
    return mainModule->registerSignal(name);
}

void EventHandler::emit(simsignal_t signalID, cPacket* s) {
    ASSERT(mainModule != NULL);
    mainModule->emit(signalID, s);
}

