//
// Generated file, do not edit! Created by opp_msgc 4.3 from messages/packets.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "packets_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(NcorpPacket);

NcorpPacket::NcorpPacket(const char *name, int kind) : cPacket(name,kind)
{
}

NcorpPacket::NcorpPacket(const NcorpPacket& other) : cPacket(other)
{
    copy(other);
}

NcorpPacket::~NcorpPacket()
{
}

NcorpPacket& NcorpPacket::operator=(const NcorpPacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void NcorpPacket::copy(const NcorpPacket& other)
{
    this->type_var = other.type_var;
}

void NcorpPacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->type_var);
}

void NcorpPacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->type_var);
}

MessageKind& NcorpPacket::getType()
{
    return type_var;
}

void NcorpPacket::setType(const MessageKind& type)
{
    this->type_var = type;
}

class NcorpPacketDescriptor : public cClassDescriptor
{
  public:
    NcorpPacketDescriptor();
    virtual ~NcorpPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NcorpPacketDescriptor);

NcorpPacketDescriptor::NcorpPacketDescriptor() : cClassDescriptor("NcorpPacket", "cPacket")
{
}

NcorpPacketDescriptor::~NcorpPacketDescriptor()
{
}

bool NcorpPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NcorpPacket *>(obj)!=NULL;
}

const char *NcorpPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NcorpPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int NcorpPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *NcorpPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "type",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int NcorpPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NcorpPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "MessageKind",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *NcorpPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NcorpPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NcorpPacket *pp = (NcorpPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NcorpPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NcorpPacket *pp = (NcorpPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getType(); return out.str();}
        default: return "";
    }
}

bool NcorpPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NcorpPacket *pp = (NcorpPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *NcorpPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "MessageKind",
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *NcorpPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NcorpPacket *pp = (NcorpPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getType()); break;
        default: return NULL;
    }
}

Register_Class(EtxProbe);

EtxProbe::EtxProbe(const char *name, int kind) : NcorpPacket(name,kind)
{
    this->setType(ETX_PROBE);
    this->setName("etx-probe");

    neighbours_arraysize = 0;
    this->neighbours_var = 0;
    this->sendTime_var = 0;
    recPackets_arraysize = 0;
    this->recPackets_var = 0;
}

EtxProbe::EtxProbe(const EtxProbe& other) : NcorpPacket(other)
{
    neighbours_arraysize = 0;
    this->neighbours_var = 0;
    recPackets_arraysize = 0;
    this->recPackets_var = 0;
    copy(other);
}

EtxProbe::~EtxProbe()
{
    delete [] neighbours_var;
    delete [] recPackets_var;
}

EtxProbe& EtxProbe::operator=(const EtxProbe& other)
{
    if (this==&other) return *this;
    NcorpPacket::operator=(other);
    copy(other);
    return *this;
}

void EtxProbe::copy(const EtxProbe& other)
{
    delete [] this->neighbours_var;
    this->neighbours_var = (other.neighbours_arraysize==0) ? NULL : new IPv4Address[other.neighbours_arraysize];
    neighbours_arraysize = other.neighbours_arraysize;
    for (unsigned int i=0; i<neighbours_arraysize; i++)
        this->neighbours_var[i] = other.neighbours_var[i];
    this->sendTime_var = other.sendTime_var;
    delete [] this->recPackets_var;
    this->recPackets_var = (other.recPackets_arraysize==0) ? NULL : new int[other.recPackets_arraysize];
    recPackets_arraysize = other.recPackets_arraysize;
    for (unsigned int i=0; i<recPackets_arraysize; i++)
        this->recPackets_var[i] = other.recPackets_var[i];
    this->position_var = other.position_var;
    this->forwardSet_var = other.forwardSet_var;
}

void EtxProbe::parsimPack(cCommBuffer *b)
{
    NcorpPacket::parsimPack(b);
    b->pack(neighbours_arraysize);
    doPacking(b,this->neighbours_var,neighbours_arraysize);
    doPacking(b,this->sendTime_var);
    b->pack(recPackets_arraysize);
    doPacking(b,this->recPackets_var,recPackets_arraysize);
    doPacking(b,this->position_var);
    doPacking(b,this->forwardSet_var);
}

void EtxProbe::parsimUnpack(cCommBuffer *b)
{
    NcorpPacket::parsimUnpack(b);
    delete [] this->neighbours_var;
    b->unpack(neighbours_arraysize);
    if (neighbours_arraysize==0) {
        this->neighbours_var = 0;
    } else {
        this->neighbours_var = new IPv4Address[neighbours_arraysize];
        doUnpacking(b,this->neighbours_var,neighbours_arraysize);
    }
    doUnpacking(b,this->sendTime_var);
    delete [] this->recPackets_var;
    b->unpack(recPackets_arraysize);
    if (recPackets_arraysize==0) {
        this->recPackets_var = 0;
    } else {
        this->recPackets_var = new int[recPackets_arraysize];
        doUnpacking(b,this->recPackets_var,recPackets_arraysize);
    }
    doUnpacking(b,this->position_var);
    doUnpacking(b,this->forwardSet_var);
}

void EtxProbe::setNeighboursArraySize(unsigned int size)
{
    IPv4Address *neighbours_var2 = (size==0) ? NULL : new IPv4Address[size];
    unsigned int sz = neighbours_arraysize < size ? neighbours_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        neighbours_var2[i] = this->neighbours_var[i];
    neighbours_arraysize = size;
    delete [] this->neighbours_var;
    this->neighbours_var = neighbours_var2;
}

unsigned int EtxProbe::getNeighboursArraySize() const
{
    return neighbours_arraysize;
}

IPv4Address& EtxProbe::getNeighbours(unsigned int k)
{
    if (k>=neighbours_arraysize) throw cRuntimeError("Array of size %d indexed by %d", neighbours_arraysize, k);
    return neighbours_var[k];
}

void EtxProbe::setNeighbours(unsigned int k, const IPv4Address& neighbours)
{
    if (k>=neighbours_arraysize) throw cRuntimeError("Array of size %d indexed by %d", neighbours_arraysize, k);
    this->neighbours_var[k] = neighbours;
}

simtime_t EtxProbe::getSendTime() const
{
    return sendTime_var;
}

void EtxProbe::setSendTime(simtime_t sendTime)
{
    this->sendTime_var = sendTime;
}

void EtxProbe::setRecPacketsArraySize(unsigned int size)
{
    int *recPackets_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = recPackets_arraysize < size ? recPackets_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        recPackets_var2[i] = this->recPackets_var[i];
    for (unsigned int i=sz; i<size; i++)
        recPackets_var2[i] = 0;
    recPackets_arraysize = size;
    delete [] this->recPackets_var;
    this->recPackets_var = recPackets_var2;
}

unsigned int EtxProbe::getRecPacketsArraySize() const
{
    return recPackets_arraysize;
}

int EtxProbe::getRecPackets(unsigned int k) const
{
    if (k>=recPackets_arraysize) throw cRuntimeError("Array of size %d indexed by %d", recPackets_arraysize, k);
    return recPackets_var[k];
}

void EtxProbe::setRecPackets(unsigned int k, int recPackets)
{
    if (k>=recPackets_arraysize) throw cRuntimeError("Array of size %d indexed by %d", recPackets_arraysize, k);
    this->recPackets_var[k] = recPackets;
}

Coord& EtxProbe::getPosition()
{
    return position_var;
}

void EtxProbe::setPosition(const Coord& position)
{
    this->position_var = position;
}

ListOfTargets& EtxProbe::getForwardSet()
{
    return forwardSet_var;
}

void EtxProbe::setForwardSet(const ListOfTargets& forwardSet)
{
    this->forwardSet_var = forwardSet;
}

class EtxProbeDescriptor : public cClassDescriptor
{
  public:
    EtxProbeDescriptor();
    virtual ~EtxProbeDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(EtxProbeDescriptor);

EtxProbeDescriptor::EtxProbeDescriptor() : cClassDescriptor("EtxProbe", "NcorpPacket")
{
}

EtxProbeDescriptor::~EtxProbeDescriptor()
{
}

bool EtxProbeDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<EtxProbe *>(obj)!=NULL;
}

const char *EtxProbeDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int EtxProbeDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int EtxProbeDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *EtxProbeDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "neighbours",
        "sendTime",
        "recPackets",
        "position",
        "forwardSet",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int EtxProbeDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbours")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sendTime")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "recPackets")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "position")==0) return base+3;
    if (fieldName[0]=='f' && strcmp(fieldName, "forwardSet")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *EtxProbeDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "IPv4Address",
        "simtime_t",
        "int",
        "Coord",
        "ListOfTargets",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *EtxProbeDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int EtxProbeDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    EtxProbe *pp = (EtxProbe *)object; (void)pp;
    switch (field) {
        case 0: return pp->getNeighboursArraySize();
        case 2: return pp->getRecPacketsArraySize();
        default: return 0;
    }
}

std::string EtxProbeDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    EtxProbe *pp = (EtxProbe *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getNeighbours(i); return out.str();}
        case 1: return double2string(pp->getSendTime());
        case 2: return long2string(pp->getRecPackets(i));
        case 3: {std::stringstream out; out << pp->getPosition(); return out.str();}
        case 4: {std::stringstream out; out << pp->getForwardSet(); return out.str();}
        default: return "";
    }
}

bool EtxProbeDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    EtxProbe *pp = (EtxProbe *)object; (void)pp;
    switch (field) {
        case 1: pp->setSendTime(string2double(value)); return true;
        case 2: pp->setRecPackets(i,string2long(value)); return true;
        default: return false;
    }
}

const char *EtxProbeDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "IPv4Address",
        NULL,
        NULL,
        "Coord",
        "ListOfTargets",
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *EtxProbeDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    EtxProbe *pp = (EtxProbe *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getNeighbours(i)); break;
        case 3: return (void *)(&pp->getPosition()); break;
        case 4: return (void *)(&pp->getForwardSet()); break;
        default: return NULL;
    }
}

Register_Class(CodedAck);

CodedAck::CodedAck(const char *name, int kind) : NcorpPacket(name,kind)
{
    this->setType(CODED_ACK);
    this->setName("coded-ack");

    this->flowId_var = 0;
    this->ackGenerationId_var = 0;
}

CodedAck::CodedAck(const CodedAck& other) : NcorpPacket(other)
{
    copy(other);
}

CodedAck::~CodedAck()
{
}

CodedAck& CodedAck::operator=(const CodedAck& other)
{
    if (this==&other) return *this;
    NcorpPacket::operator=(other);
    copy(other);
    return *this;
}

void CodedAck::copy(const CodedAck& other)
{
    this->flowId_var = other.flowId_var;
    this->ackGenerationId_var = other.ackGenerationId_var;
    this->ackVector_var = other.ackVector_var;
}

void CodedAck::parsimPack(cCommBuffer *b)
{
    NcorpPacket::parsimPack(b);
    doPacking(b,this->flowId_var);
    doPacking(b,this->ackGenerationId_var);
    doPacking(b,this->ackVector_var);
}

void CodedAck::parsimUnpack(cCommBuffer *b)
{
    NcorpPacket::parsimUnpack(b);
    doUnpacking(b,this->flowId_var);
    doUnpacking(b,this->ackGenerationId_var);
    doUnpacking(b,this->ackVector_var);
}

uint16_t CodedAck::getFlowId() const
{
    return flowId_var;
}

void CodedAck::setFlowId(uint16_t flowId)
{
    this->flowId_var = flowId;
}

uint16_t CodedAck::getAckGenerationId() const
{
    return ackGenerationId_var;
}

void CodedAck::setAckGenerationId(uint16_t ackGenerationId)
{
    this->ackGenerationId_var = ackGenerationId;
}

AckVector& CodedAck::getAckVector()
{
    return ackVector_var;
}

void CodedAck::setAckVector(const AckVector& ackVector)
{
    this->ackVector_var = ackVector;
}

class CodedAckDescriptor : public cClassDescriptor
{
  public:
    CodedAckDescriptor();
    virtual ~CodedAckDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(CodedAckDescriptor);

CodedAckDescriptor::CodedAckDescriptor() : cClassDescriptor("CodedAck", "NcorpPacket")
{
}

CodedAckDescriptor::~CodedAckDescriptor()
{
}

bool CodedAckDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CodedAck *>(obj)!=NULL;
}

const char *CodedAckDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CodedAckDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int CodedAckDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *CodedAckDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "flowId",
        "ackGenerationId",
        "ackVector",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int CodedAckDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flowId")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackGenerationId")==0) return base+1;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackVector")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CodedAckDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "uint16_t",
        "uint16_t",
        "AckVector",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *CodedAckDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CodedAckDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CodedAck *pp = (CodedAck *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CodedAckDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CodedAck *pp = (CodedAck *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getFlowId());
        case 1: return ulong2string(pp->getAckGenerationId());
        case 2: {std::stringstream out; out << pp->getAckVector(); return out.str();}
        default: return "";
    }
}

bool CodedAckDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CodedAck *pp = (CodedAck *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlowId(string2ulong(value)); return true;
        case 1: pp->setAckGenerationId(string2ulong(value)); return true;
        default: return false;
    }
}

const char *CodedAckDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        "AckVector",
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *CodedAckDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CodedAck *pp = (CodedAck *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getAckVector()); break;
        default: return NULL;
    }
}

Register_Class(CodedEAck);

CodedEAck::CodedEAck(const char *name, int kind) : NcorpPacket(name,kind)
{
    this->setType(CODED_EACK);
    this->setName("coded-eack");

    this->flowId_var = 0;
    this->generationId_var = 0;
}

CodedEAck::CodedEAck(const CodedEAck& other) : NcorpPacket(other)
{
    copy(other);
}

CodedEAck::~CodedEAck()
{
}

CodedEAck& CodedEAck::operator=(const CodedEAck& other)
{
    if (this==&other) return *this;
    NcorpPacket::operator=(other);
    copy(other);
    return *this;
}

void CodedEAck::copy(const CodedEAck& other)
{
    this->flowId_var = other.flowId_var;
    this->generationId_var = other.generationId_var;
    this->nextHopAddr_var = other.nextHopAddr_var;
}

void CodedEAck::parsimPack(cCommBuffer *b)
{
    NcorpPacket::parsimPack(b);
    doPacking(b,this->flowId_var);
    doPacking(b,this->generationId_var);
    doPacking(b,this->nextHopAddr_var);
}

void CodedEAck::parsimUnpack(cCommBuffer *b)
{
    NcorpPacket::parsimUnpack(b);
    doUnpacking(b,this->flowId_var);
    doUnpacking(b,this->generationId_var);
    doUnpacking(b,this->nextHopAddr_var);
}

uint16_t CodedEAck::getFlowId() const
{
    return flowId_var;
}

void CodedEAck::setFlowId(uint16_t flowId)
{
    this->flowId_var = flowId;
}

uint16_t CodedEAck::getGenerationId() const
{
    return generationId_var;
}

void CodedEAck::setGenerationId(uint16_t generationId)
{
    this->generationId_var = generationId;
}

IPv4Address& CodedEAck::getNextHopAddr()
{
    return nextHopAddr_var;
}

void CodedEAck::setNextHopAddr(const IPv4Address& nextHopAddr)
{
    this->nextHopAddr_var = nextHopAddr;
}

class CodedEAckDescriptor : public cClassDescriptor
{
  public:
    CodedEAckDescriptor();
    virtual ~CodedEAckDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(CodedEAckDescriptor);

CodedEAckDescriptor::CodedEAckDescriptor() : cClassDescriptor("CodedEAck", "NcorpPacket")
{
}

CodedEAckDescriptor::~CodedEAckDescriptor()
{
}

bool CodedEAckDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CodedEAck *>(obj)!=NULL;
}

const char *CodedEAckDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CodedEAckDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int CodedEAckDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *CodedEAckDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "flowId",
        "generationId",
        "nextHopAddr",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int CodedEAckDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flowId")==0) return base+0;
    if (fieldName[0]=='g' && strcmp(fieldName, "generationId")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextHopAddr")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CodedEAckDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "uint16_t",
        "uint16_t",
        "IPv4Address",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *CodedEAckDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CodedEAckDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CodedEAck *pp = (CodedEAck *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CodedEAckDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CodedEAck *pp = (CodedEAck *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getFlowId());
        case 1: return ulong2string(pp->getGenerationId());
        case 2: {std::stringstream out; out << pp->getNextHopAddr(); return out.str();}
        default: return "";
    }
}

bool CodedEAckDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CodedEAck *pp = (CodedEAck *)object; (void)pp;
    switch (field) {
        case 0: pp->setFlowId(string2ulong(value)); return true;
        case 1: pp->setGenerationId(string2ulong(value)); return true;
        default: return false;
    }
}

const char *CodedEAckDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        "IPv4Address",
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *CodedEAckDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CodedEAck *pp = (CodedEAck *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getNextHopAddr()); break;
        default: return NULL;
    }
}

Register_Class(CodedDataAck);

CodedDataAck::CodedDataAck(const char *name, int kind) : NcorpPacket(name,kind)
{
    this->setType(CODED_DATA_ACK);
    this->setName("coded-data-ack");

    this->flowId_var = 0;
    this->baseWindow_var = 0;
    this->generationId_var = 0;
    this->ackGenerationId_var = 0;
    this->useAck_var = 0;
    this->dbl_var = 0;
    this->inovative_var = false;
}

CodedDataAck::CodedDataAck(const CodedDataAck& other) : NcorpPacket(other)
{
    copy(other);
}

CodedDataAck::~CodedDataAck()
{
}

CodedDataAck& CodedDataAck::operator=(const CodedDataAck& other)
{
    if (this==&other) return *this;
    NcorpPacket::operator=(other);
    copy(other);
    return *this;
}

void CodedDataAck::copy(const CodedDataAck& other)
{
    this->flowSrcAddr_var = other.flowSrcAddr_var;
    this->flowDstAddr_var = other.flowDstAddr_var;
    this->flowId_var = other.flowId_var;
    this->baseWindow_var = other.baseWindow_var;
    this->generationId_var = other.generationId_var;
    this->ackGenerationId_var = other.ackGenerationId_var;
    this->useAck_var = other.useAck_var;
    this->dbl_var = other.dbl_var;
    this->forwardSet_var = other.forwardSet_var;
    this->ackVector_var = other.ackVector_var;
    this->encodingVector_var = other.encodingVector_var;
    this->payload_var = other.payload_var;
    this->payloadSize_var = other.payloadSize_var;
    this->inovative_var = other.inovative_var;
}

void CodedDataAck::parsimPack(cCommBuffer *b)
{
    NcorpPacket::parsimPack(b);
    doPacking(b,this->flowSrcAddr_var);
    doPacking(b,this->flowDstAddr_var);
    doPacking(b,this->flowId_var);
    doPacking(b,this->baseWindow_var);
    doPacking(b,this->generationId_var);
    doPacking(b,this->ackGenerationId_var);
    doPacking(b,this->useAck_var);
    doPacking(b,this->dbl_var);
    doPacking(b,this->forwardSet_var);
    doPacking(b,this->ackVector_var);
    doPacking(b,this->encodingVector_var);
    doPacking(b,this->payload_var);
    doPacking(b,this->payloadSize_var);
    doPacking(b,this->inovative_var);
}

void CodedDataAck::parsimUnpack(cCommBuffer *b)
{
    NcorpPacket::parsimUnpack(b);
    doUnpacking(b,this->flowSrcAddr_var);
    doUnpacking(b,this->flowDstAddr_var);
    doUnpacking(b,this->flowId_var);
    doUnpacking(b,this->baseWindow_var);
    doUnpacking(b,this->generationId_var);
    doUnpacking(b,this->ackGenerationId_var);
    doUnpacking(b,this->useAck_var);
    doUnpacking(b,this->dbl_var);
    doUnpacking(b,this->forwardSet_var);
    doUnpacking(b,this->ackVector_var);
    doUnpacking(b,this->encodingVector_var);
    doUnpacking(b,this->payload_var);
    doUnpacking(b,this->payloadSize_var);
    doUnpacking(b,this->inovative_var);
}

IPv4Address& CodedDataAck::getFlowSrcAddr()
{
    return flowSrcAddr_var;
}

void CodedDataAck::setFlowSrcAddr(const IPv4Address& flowSrcAddr)
{
    this->flowSrcAddr_var = flowSrcAddr;
}

IPv4Address& CodedDataAck::getFlowDstAddr()
{
    return flowDstAddr_var;
}

void CodedDataAck::setFlowDstAddr(const IPv4Address& flowDstAddr)
{
    this->flowDstAddr_var = flowDstAddr;
}

uint16_t CodedDataAck::getFlowId() const
{
    return flowId_var;
}

void CodedDataAck::setFlowId(uint16_t flowId)
{
    this->flowId_var = flowId;
}

uint16_t CodedDataAck::getBaseWindow() const
{
    return baseWindow_var;
}

void CodedDataAck::setBaseWindow(uint16_t baseWindow)
{
    this->baseWindow_var = baseWindow;
}

uint16_t CodedDataAck::getGenerationId() const
{
    return generationId_var;
}

void CodedDataAck::setGenerationId(uint16_t generationId)
{
    this->generationId_var = generationId;
}

uint16_t CodedDataAck::getAckGenerationId() const
{
    return ackGenerationId_var;
}

void CodedDataAck::setAckGenerationId(uint16_t ackGenerationId)
{
    this->ackGenerationId_var = ackGenerationId;
}

bool CodedDataAck::hasAck() const
{
    return useAck_var;
}

void CodedDataAck::setUseAck(bool useAck)
{
    this->useAck_var = useAck;
}

double CodedDataAck::getDbl() const
{
    return dbl_var;
}

void CodedDataAck::setDbl(double dbl)
{
    this->dbl_var = dbl;
}

ForwardSet& CodedDataAck::getForwardSet()
{
    return forwardSet_var;
}

void CodedDataAck::setForwardSet(const ForwardSet& forwardSet)
{
    this->forwardSet_var = forwardSet;
}

AckVector& CodedDataAck::getAckVector()
{
    return ackVector_var;
}

void CodedDataAck::setAckVector(const AckVector& ackVector)
{
    this->ackVector_var = ackVector;
}

EncodingVector& CodedDataAck::getEncodingVector()
{
    return encodingVector_var;
}

void CodedDataAck::setEncodingVector(const EncodingVector& encodingVector)
{
    this->encodingVector_var = encodingVector;
}

Payload& CodedDataAck::getPayload()
{
    return payload_var;
}

void CodedDataAck::setPayload(const Payload& payload)
{
    this->payload_var = payload;
}

size_t& CodedDataAck::getPayloadSize()
{
    return payloadSize_var;
}

void CodedDataAck::setPayloadSize(const size_t& payloadSize)
{
    this->payloadSize_var = payloadSize;
}

bool CodedDataAck::isInovative() const
{
    return inovative_var;
}

void CodedDataAck::setInovative(bool inovative)
{
    this->inovative_var = inovative;
}

class CodedDataAckDescriptor : public cClassDescriptor
{
  public:
    CodedDataAckDescriptor();
    virtual ~CodedDataAckDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(CodedDataAckDescriptor);

CodedDataAckDescriptor::CodedDataAckDescriptor() : cClassDescriptor("CodedDataAck", "NcorpPacket")
{
}

CodedDataAckDescriptor::~CodedDataAckDescriptor()
{
}

bool CodedDataAckDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CodedDataAck *>(obj)!=NULL;
}

const char *CodedDataAckDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CodedDataAckDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 14+basedesc->getFieldCount(object) : 14;
}

unsigned int CodedDataAckDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<14) ? fieldTypeFlags[field] : 0;
}

const char *CodedDataAckDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "flowSrcAddr",
        "flowDstAddr",
        "flowId",
        "baseWindow",
        "generationId",
        "ackGenerationId",
        "useAck",
        "dbl",
        "forwardSet",
        "ackVector",
        "encodingVector",
        "payload",
        "payloadSize",
        "inovative",
    };
    return (field>=0 && field<14) ? fieldNames[field] : NULL;
}

int CodedDataAckDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flowSrcAddr")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flowDstAddr")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "flowId")==0) return base+2;
    if (fieldName[0]=='b' && strcmp(fieldName, "baseWindow")==0) return base+3;
    if (fieldName[0]=='g' && strcmp(fieldName, "generationId")==0) return base+4;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackGenerationId")==0) return base+5;
    if (fieldName[0]=='u' && strcmp(fieldName, "useAck")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "dbl")==0) return base+7;
    if (fieldName[0]=='f' && strcmp(fieldName, "forwardSet")==0) return base+8;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackVector")==0) return base+9;
    if (fieldName[0]=='e' && strcmp(fieldName, "encodingVector")==0) return base+10;
    if (fieldName[0]=='p' && strcmp(fieldName, "payload")==0) return base+11;
    if (fieldName[0]=='p' && strcmp(fieldName, "payloadSize")==0) return base+12;
    if (fieldName[0]=='i' && strcmp(fieldName, "inovative")==0) return base+13;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CodedDataAckDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "IPv4Address",
        "IPv4Address",
        "uint16_t",
        "uint16_t",
        "uint16_t",
        "uint16_t",
        "bool",
        "double",
        "ForwardSet",
        "AckVector",
        "EncodingVector",
        "Payload",
        "size_t",
        "bool",
    };
    return (field>=0 && field<14) ? fieldTypeStrings[field] : NULL;
}

const char *CodedDataAckDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 6:
            if (!strcmp(propertyname,"getter")) return "hasAck";
            return NULL;
        case 13:
            if (!strcmp(propertyname,"getter")) return "isInovative";
            return NULL;
        default: return NULL;
    }
}

int CodedDataAckDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CodedDataAck *pp = (CodedDataAck *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CodedDataAckDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CodedDataAck *pp = (CodedDataAck *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getFlowSrcAddr(); return out.str();}
        case 1: {std::stringstream out; out << pp->getFlowDstAddr(); return out.str();}
        case 2: return ulong2string(pp->getFlowId());
        case 3: return ulong2string(pp->getBaseWindow());
        case 4: return ulong2string(pp->getGenerationId());
        case 5: return ulong2string(pp->getAckGenerationId());
        case 6: return bool2string(pp->hasAck());
        case 7: return double2string(pp->getDbl());
        case 8: {std::stringstream out; out << pp->getForwardSet(); return out.str();}
        case 9: {std::stringstream out; out << pp->getAckVector(); return out.str();}
        case 10: {std::stringstream out; out << pp->getEncodingVector(); return out.str();}
        case 11: {std::stringstream out; out << pp->getPayload(); return out.str();}
        case 12: {std::stringstream out; out << pp->getPayloadSize(); return out.str();}
        case 13: return bool2string(pp->isInovative());
        default: return "";
    }
}

bool CodedDataAckDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CodedDataAck *pp = (CodedDataAck *)object; (void)pp;
    switch (field) {
        case 2: pp->setFlowId(string2ulong(value)); return true;
        case 3: pp->setBaseWindow(string2ulong(value)); return true;
        case 4: pp->setGenerationId(string2ulong(value)); return true;
        case 5: pp->setAckGenerationId(string2ulong(value)); return true;
        case 6: pp->setUseAck(string2bool(value)); return true;
        case 7: pp->setDbl(string2double(value)); return true;
        case 13: pp->setInovative(string2bool(value)); return true;
        default: return false;
    }
}

const char *CodedDataAckDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "IPv4Address",
        "IPv4Address",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        "ForwardSet",
        "AckVector",
        "EncodingVector",
        "Payload",
        "size_t",
        NULL,
    };
    return (field>=0 && field<14) ? fieldStructNames[field] : NULL;
}

void *CodedDataAckDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CodedDataAck *pp = (CodedDataAck *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getFlowSrcAddr()); break;
        case 1: return (void *)(&pp->getFlowDstAddr()); break;
        case 8: return (void *)(&pp->getForwardSet()); break;
        case 9: return (void *)(&pp->getAckVector()); break;
        case 10: return (void *)(&pp->getEncodingVector()); break;
        case 11: return (void *)(&pp->getPayload()); break;
        case 12: return (void *)(&pp->getPayloadSize()); break;
        default: return NULL;
    }
}


