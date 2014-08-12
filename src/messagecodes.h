/*
 * messagecodes.h
 *
 *  Created on: Dec 25, 2012
 *      Author: ivocalado
 */

#ifndef MESSAGECODES_H_
#define MESSAGECODES_H_



enum Role {
    SENDER = 1,
    RELAY,
    RECEIVER
};

enum FlowStatus {
    ACTIVE = 1,
    WAITING,
    CLOSED,
    INVALID
};


#endif /* MESSAGECODES_H_ */
