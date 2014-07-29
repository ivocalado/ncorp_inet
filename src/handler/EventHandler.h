/*
 * EventHandler.h
 *
 *  Created on: Jun 3, 2014
 *      Author: ivocalado
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <omnetpp.h>
#include "IPvXAddress.h"
#include "Coord.h"
class Ncorp;

class EventHandler {
protected:
    Ncorp* mainModule;
    virtual void scheduleTimer(simtime_t when, cMessage* msg);
    virtual short int getModuleKind() = 0;
    virtual const char* getModuleName() = 0;
    virtual IPv4Address getMyNetAddr() const;
    virtual Coord getMyPosition() const;
    virtual simsignal_t registerSignal(const char* name);

    cPar& par(const char *parname);
    cMessage* cancelEvent(cMessage* msg);
    void emit(simsignal_t signalID, cPacket* s);
public:
    EventHandler();
    void setMainModule(Ncorp* mainModule);
    virtual ~EventHandler();
    virtual void processAppCommand(cMessage* msg) { delete msg;}
    virtual void processNetPck(IPv4Address from, cMessage* msg) {delete msg;}

    virtual void processTimer(cMessage* msg) {delete msg;};
    virtual void initialize() { ASSERT(mainModule != NULL);};
    virtual void finish() {};
    virtual void sendDown(cPacket* pck, IPv4Address dest);
};

#endif /* EVENTHANDLER_H_ */
