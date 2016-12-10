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

#include "NcorpManager.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/networklayer/ipv4/IIPv4RoutingTable.h"
#include "SaorsManager.h"
#include "inet/routing/extras/base/ControlManetRouting_m.h"
#include  "inet/networklayer/ipv4/IPv4Datagram.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/ipv4/IPv4InterfaceData.h"


Define_Module(NcorpManager);
NcorpManager::NcorpManager() {
    // TODO Auto-generated constructor stub

}

NcorpManager::~NcorpManager() {
    // TODO Auto-generated destructor stub
}

/** @brief initialization function */
void NcorpManager::initialize(int stage) {
    if(stage == 4){
    }
}

#include<iostream>
using namespace std;

/** @brief handles received messages - forwards them to the routing module */
void NcorpManager::handleMessage(cMessage *msg) {
    cout<<msg<<endl;
}


