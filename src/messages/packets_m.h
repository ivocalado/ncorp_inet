//
// Generated file, do not edit! Created by opp_msgc 4.3 from messages/packets.msg.
//

#ifndef _PACKETS_M_H_
#define _PACKETS_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0403
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "IPvXAddress.h"
#include "Coord.h"
#include <vector>
#include <memory>




enum MessageKind {
	ETX_PROBE = 1,
	CODED_ACK,
	CODED_EACK,
	CODED_DATA_ACK,
	
	NCORP_I_DATA,
	NCORP_I_ERROR_NODE_UNREACHABLE,
	FLUSH_STREAM,
    INVALID_FLOW, 
    RESPONSE_FLOW_CREATION,
    NO_ROUTE,
    NO_ENOUGH_BANDWIDTH,
    SEND_NEW_PACKET,
    GENERATION_TIMEOUT,
	
    LAST_NCORP_MESSAGE_KIND,
};



enum ModuleKind {
	ETX_MODULE = 1,
	CCACK_MODULE
};

struct Target {
	IPv4Address addr;
	Coord position;
};

typedef std::list<Target> ListOfTargets;
typedef std::list<IPv4Address> ForwardSet;
typedef std::shared_ptr<std::vector<uint8_t> > Payload;
typedef std::vector<uint8_t> AckVector;
typedef std::vector<uint8_t> EncodingVector;
// }}



/**
 * Class generated from <tt>messages/packets.msg</tt> by opp_msgc.
 * <pre>
 * packet NcorpPacket {
 *     MessageKind type;
 * }
 * </pre>
 */
class NcorpPacket : public ::cPacket
{
  protected:
    MessageKind type_var;

  private:
    void copy(const NcorpPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NcorpPacket&);

  public:
    NcorpPacket(const char *name=NULL, int kind=0);
    NcorpPacket(const NcorpPacket& other);
    virtual ~NcorpPacket();
    NcorpPacket& operator=(const NcorpPacket& other);
    virtual NcorpPacket *dup() const {return new NcorpPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual MessageKind& getType();
    virtual const MessageKind& getType() const {return const_cast<NcorpPacket*>(this)->getType();}
    virtual void setType(const MessageKind& type);
};

inline void doPacking(cCommBuffer *b, NcorpPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, NcorpPacket& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>messages/packets.msg</tt> by opp_msgc.
 * <pre>
 * packet EtxProbe extends NcorpPacket
 * {
 * 	type = ETX_PROBE;
 *     name = "etx-probe";
 *     IPv4Address neighbours[];
 *     simtime_t sendTime;
 *     int recPackets[];
 *     Coord position;
 *     ListOfTargets forwardSet; 
 * }
 * </pre>
 */
class EtxProbe : public ::NcorpPacket
{
  protected:
    IPv4Address *neighbours_var; // array ptr
    unsigned int neighbours_arraysize;
    simtime_t sendTime_var;
    int *recPackets_var; // array ptr
    unsigned int recPackets_arraysize;
    Coord position_var;
    ListOfTargets forwardSet_var;

  private:
    void copy(const EtxProbe& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const EtxProbe&);

  public:
    EtxProbe(const char *name=NULL, int kind=0);
    EtxProbe(const EtxProbe& other);
    virtual ~EtxProbe();
    EtxProbe& operator=(const EtxProbe& other);
    virtual EtxProbe *dup() const {return new EtxProbe(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual void setNeighboursArraySize(unsigned int size);
    virtual unsigned int getNeighboursArraySize() const;
    virtual IPv4Address& getNeighbours(unsigned int k);
    virtual const IPv4Address& getNeighbours(unsigned int k) const {return const_cast<EtxProbe*>(this)->getNeighbours(k);}
    virtual void setNeighbours(unsigned int k, const IPv4Address& neighbours);
    virtual simtime_t getSendTime() const;
    virtual void setSendTime(simtime_t sendTime);
    virtual void setRecPacketsArraySize(unsigned int size);
    virtual unsigned int getRecPacketsArraySize() const;
    virtual int getRecPackets(unsigned int k) const;
    virtual void setRecPackets(unsigned int k, int recPackets);
    virtual Coord& getPosition();
    virtual const Coord& getPosition() const {return const_cast<EtxProbe*>(this)->getPosition();}
    virtual void setPosition(const Coord& position);
    virtual ListOfTargets& getForwardSet();
    virtual const ListOfTargets& getForwardSet() const {return const_cast<EtxProbe*>(this)->getForwardSet();}
    virtual void setForwardSet(const ListOfTargets& forwardSet);
};

inline void doPacking(cCommBuffer *b, EtxProbe& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, EtxProbe& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>messages/packets.msg</tt> by opp_msgc.
 * <pre>
 * packet CodedAck extends NcorpPacket
 * {
 *     type = CODED_ACK;
 *     name = "coded-ack";
 *     uint16_t flowId;
 *     uint16_t ackGenerationId;
 *     AckVector ackVector;
 * }
 * </pre>
 */
class CodedAck : public ::NcorpPacket
{
  protected:
    uint16_t flowId_var;
    uint16_t ackGenerationId_var;
    AckVector ackVector_var;

  private:
    void copy(const CodedAck& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CodedAck&);

  public:
    CodedAck(const char *name=NULL, int kind=0);
    CodedAck(const CodedAck& other);
    virtual ~CodedAck();
    CodedAck& operator=(const CodedAck& other);
    virtual CodedAck *dup() const {return new CodedAck(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual uint16_t getFlowId() const;
    virtual void setFlowId(uint16_t flowId);
    virtual uint16_t getAckGenerationId() const;
    virtual void setAckGenerationId(uint16_t ackGenerationId);
    virtual AckVector& getAckVector();
    virtual const AckVector& getAckVector() const {return const_cast<CodedAck*>(this)->getAckVector();}
    virtual void setAckVector(const AckVector& ackVector);
};

inline void doPacking(cCommBuffer *b, CodedAck& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, CodedAck& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>messages/packets.msg</tt> by opp_msgc.
 * <pre>
 * packet CodedEAck extends NcorpPacket
 * {
 *     type = CODED_EACK;
 *     name = "coded-eack";
 *     uint16_t flowId;
 *     uint16_t generationId;
 *     IPv4Address nextHopAddr;
 * }
 * </pre>
 */
class CodedEAck : public ::NcorpPacket
{
  protected:
    uint16_t flowId_var;
    uint16_t generationId_var;
    IPv4Address nextHopAddr_var;

  private:
    void copy(const CodedEAck& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CodedEAck&);

  public:
    CodedEAck(const char *name=NULL, int kind=0);
    CodedEAck(const CodedEAck& other);
    virtual ~CodedEAck();
    CodedEAck& operator=(const CodedEAck& other);
    virtual CodedEAck *dup() const {return new CodedEAck(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual uint16_t getFlowId() const;
    virtual void setFlowId(uint16_t flowId);
    virtual uint16_t getGenerationId() const;
    virtual void setGenerationId(uint16_t generationId);
    virtual IPv4Address& getNextHopAddr();
    virtual const IPv4Address& getNextHopAddr() const {return const_cast<CodedEAck*>(this)->getNextHopAddr();}
    virtual void setNextHopAddr(const IPv4Address& nextHopAddr);
};

inline void doPacking(cCommBuffer *b, CodedEAck& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, CodedEAck& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>messages/packets.msg</tt> by opp_msgc.
 * <pre>
 * packet CodedDataAck extends NcorpPacket
 * {
 *     type = CODED_DATA_ACK;
 *     name = "coded-data-ack";
 *     IPv4Address flowSrcAddr; 
 *     IPv4Address flowDstAddr; 
 *     uint16_t flowId;
 *     uint16_t baseWindow;
 *     uint16_t generationId;
 *     uint16_t ackGenerationId;
 *     bool useAck @getter(hasAck);
 *     
 *     double dbl;
 *     ForwardSet forwardSet; 
 *     AckVector ackVector;
 *     EncodingVector encodingVector; 
 *     
 *     Payload payload;
 *     size_t payloadSize;
 * }
 * </pre>
 */
class CodedDataAck : public ::NcorpPacket
{
  protected:
    IPv4Address flowSrcAddr_var;
    IPv4Address flowDstAddr_var;
    uint16_t flowId_var;
    uint16_t baseWindow_var;
    uint16_t generationId_var;
    uint16_t ackGenerationId_var;
    bool useAck_var;
    double dbl_var;
    ForwardSet forwardSet_var;
    AckVector ackVector_var;
    EncodingVector encodingVector_var;
    Payload payload_var;
    size_t payloadSize_var;

  private:
    void copy(const CodedDataAck& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CodedDataAck&);

  public:
    CodedDataAck(const char *name=NULL, int kind=0);
    CodedDataAck(const CodedDataAck& other);
    virtual ~CodedDataAck();
    CodedDataAck& operator=(const CodedDataAck& other);
    virtual CodedDataAck *dup() const {return new CodedDataAck(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual IPv4Address& getFlowSrcAddr();
    virtual const IPv4Address& getFlowSrcAddr() const {return const_cast<CodedDataAck*>(this)->getFlowSrcAddr();}
    virtual void setFlowSrcAddr(const IPv4Address& flowSrcAddr);
    virtual IPv4Address& getFlowDstAddr();
    virtual const IPv4Address& getFlowDstAddr() const {return const_cast<CodedDataAck*>(this)->getFlowDstAddr();}
    virtual void setFlowDstAddr(const IPv4Address& flowDstAddr);
    virtual uint16_t getFlowId() const;
    virtual void setFlowId(uint16_t flowId);
    virtual uint16_t getBaseWindow() const;
    virtual void setBaseWindow(uint16_t baseWindow);
    virtual uint16_t getGenerationId() const;
    virtual void setGenerationId(uint16_t generationId);
    virtual uint16_t getAckGenerationId() const;
    virtual void setAckGenerationId(uint16_t ackGenerationId);
    virtual bool hasAck() const;
    virtual void setUseAck(bool useAck);
    virtual double getDbl() const;
    virtual void setDbl(double dbl);
    virtual ForwardSet& getForwardSet();
    virtual const ForwardSet& getForwardSet() const {return const_cast<CodedDataAck*>(this)->getForwardSet();}
    virtual void setForwardSet(const ForwardSet& forwardSet);
    virtual AckVector& getAckVector();
    virtual const AckVector& getAckVector() const {return const_cast<CodedDataAck*>(this)->getAckVector();}
    virtual void setAckVector(const AckVector& ackVector);
    virtual EncodingVector& getEncodingVector();
    virtual const EncodingVector& getEncodingVector() const {return const_cast<CodedDataAck*>(this)->getEncodingVector();}
    virtual void setEncodingVector(const EncodingVector& encodingVector);
    virtual Payload& getPayload();
    virtual const Payload& getPayload() const {return const_cast<CodedDataAck*>(this)->getPayload();}
    virtual void setPayload(const Payload& payload);
    virtual size_t& getPayloadSize();
    virtual const size_t& getPayloadSize() const {return const_cast<CodedDataAck*>(this)->getPayloadSize();}
    virtual void setPayloadSize(const size_t& payloadSize);
};

inline void doPacking(cCommBuffer *b, CodedDataAck& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, CodedDataAck& obj) {obj.parsimUnpack(b);}


#endif // _PACKETS_M_H_
