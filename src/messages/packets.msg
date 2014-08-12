
cplusplus {{
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
    INVALID_FLOW, //indica que o flowId passado é invalido
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

}}

class noncobject IPv4Address;
class noncobject Coord;
class noncobject ListOfTargets;
class noncobject EncodingVector;
class noncobject AckVector;
class noncobject Payload;
class noncobject size_t;
class noncobject ForwardSet;
class noncobject MessageKind;


packet NcorpPacket {
    MessageKind type;
}

packet EtxProbe extends NcorpPacket
{
	type = ETX_PROBE;
    name = "etx-probe";
    IPv4Address neighbours[];
    simtime_t sendTime;
    int recPackets[];
    Coord position;
    ListOfTargets forwardSet; 
}

//Tamanho do pacote: 192bits + N*32 bits + 256bits (opcional) + 256bits + carga util
packet CodedAck extends NcorpPacket
{
    type = CODED_ACK;
    name = "coded-ack";
    uint16_t flowId;
    uint16_t ackGenerationId;
    AckVector ackVector;
}

packet CodedEAck extends NcorpPacket
{
    type = CODED_EACK;
    name = "coded-eack";
    uint16_t flowId;
    uint16_t generationId;
    IPv4Address nextHopAddr;
}

packet CodedDataAck extends NcorpPacket
{
    type = CODED_DATA_ACK;
    name = "coded-data-ack";
    IPv4Address flowSrcAddr; //Especifica a origem do fluxo
    IPv4Address flowDstAddr; //Especifica o destino do fluxo
    uint16_t flowId;
    uint16_t baseWindow;
    uint16_t generationId;
    uint16_t ackGenerationId;
    bool useAck @getter(hasAck);
    //uint8_t forwardSetSize #pega-se automaticamente do vetor de encaminhadores
    double dbl;
    ForwardSet forwardSet; 
    AckVector ackVector;
    EncodingVector encodingVector; //Esta estrutura é utilizada apenas para debug, visto que o coeficiente é 
    //repassado diretamente no objeto payload 
    Payload payload;
    size_t payloadSize;
}

