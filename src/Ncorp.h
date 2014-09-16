//
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
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


#ifndef __NCORP_H
#define __NCORP_H

#include <vector>
#include <map>

#include "INETDefs.h"

#include "AppBase.h"
#include "UDPSocket.h"
#include "Coord.h"
#include "CcackBaseline.h"

class ETXMetric;
class Ieee80211Mac;

/**
 * UDP application. See NED for more info.
 */
class INET_API Ncorp : public AppBase
{
  public:


  protected:


    UDPSocket socket;
    int connectionPort;
    int outputInterface;
    ETXMetric* metric;
    std::shared_ptr<ncorp::CcackBaseline> ccackBaseline;
    cGate* appOutputGate;
    cGate* appInputGate;

  protected:
    virtual int numInitStages() const {return 4;}
    virtual void initialize(int stage);
    virtual void handleMessageWhenUp(cMessage *msg);
    virtual void finish();

    //AppBase:
    virtual bool startApp(IDoneCallback *doneCallback);
    virtual bool stopApp(IDoneCallback *doneCallback);
    virtual bool crashApp(IDoneCallback *doneCallback);

    void handleMacControlMsg(cMessage* msg);
    void sendControlMsgToMac(cMessage* msg);
    virtual void handleMessage(cMessage *msg);

    virtual void handleLowerMsg(cMessage *);
    virtual void handleSelfMsg(cMessage* msg);
    bool isUpstream(IPv4Address relay, IPv4Address target);
    void handleEAckPkt(CodedEAck* packet, IPv4Address from);
    void handleCodedAckPkt(CodedAck* packet, IPv4Address from);
    simtime_t handleCodedDataAckPkt(CodedDataAck* packet, IPv4Address from);

    void deliverReceivedBlock(uint16_t flowId, IPv4Address from, std::shared_ptr<std::vector<uint8_t> > block);
    void askForChannelOpportunity();

    Ieee80211Mac* macModule;


  public:
    std::list<IPv4Address> findCandidateSet(IPv4Address target);//Retorna o conjunto de candidatos baseados na localização geográfica dos nos
    Ncorp();
    ~Ncorp();

    void scheduleTimer(simtime_t when, cMessage* msg);
    IPv4Address getMyNetAddr() const;
    string getMyNodeName();
    Coord getCurrentPosition();
    double getTransmissionTime();//Calcula o tempo médio trnamissão de um pacote de um nó vizinho para o atual
    void sendToIp(cPacket* pkt, IPv4Address dst);
};

#endif

