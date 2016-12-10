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

#ifndef HOOKS_NCORPTRANSPORTLAYERHOOK_H_
#define HOOKS_NCORPTRANSPORTLAYERHOOK_H_

#include "inet/common/INETDefs.h"
#include "inet/common/ProtocolMap.h"
#include "inet/networklayer/contract/INetworkProtocol.h"

using namespace inet;

class NcorpTransportLayerHook: public cSimpleModule {
public:
    NcorpTransportLayerHook();
    virtual ~NcorpTransportLayerHook();
protected:
    ProtocolMapping mapping;    // where to send packets after decapsulation

    /** @brief return the number of stages in the initialization */
    int numInitStages() const {
        return 5;
    }

    /** @brief initialization function */
    void initialize(int stage);

    /** @brief handles received messages - forwards them to the routing module */
    virtual void handleMessage(cMessage *msg);

    virtual void handleMessageFromHL(cMessage *msg, int gateIndex);

    virtual void handleMessageFromLL(cMessage *msg, int gateIndex);

    virtual void handleSelfMessage(cMessage *msg);

    virtual void sendUp(cMessage* msg, int gateIndex);

    virtual void sendDown(cMessage* msg, int gateIndex);

    /** @brief finish function */
    virtual void finish() {
    }
};

#endif /* HOOKS_NCORPTRANSPORTLAYERHOOK_H_ */
