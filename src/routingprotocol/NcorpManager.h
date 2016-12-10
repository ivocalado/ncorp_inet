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

#ifndef ROUTINGPROTOCOL_NCORPMANAGER_H_
#define ROUTINGPROTOCOL_NCORPMANAGER_H_

#include "inet/common/INETDefs.h"
using namespace inet;




class NcorpManager: public omnetpp::cSimpleModule {
public:
    NcorpManager();
    virtual ~NcorpManager();

protected:
  /** @brief return the number of stages in the initialization */
  int numInitStages() const  {return 5;}

  /** @brief initialization function */
  void initialize(int stage);

  /** @brief handles received messages - forwards them to the routing module */
  virtual void handleMessage(cMessage *msg);

  /** @brief finish function */
  virtual void finish() {};

};

#endif /* ROUTINGPROTOCOL_NCORPMANAGER_H_ */
