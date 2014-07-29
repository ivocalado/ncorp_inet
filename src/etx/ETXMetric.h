/*
 * ETXMetric.h
 *
 *  Created on: Jun 3, 2014
 *      Author: ivocalado
 */

#ifndef ETXMETRIC_H_
#define ETXMETRIC_H_

#include "EventHandler.h"
#include <map>
#include <set>
#include "Coord.h"
#include "IPvXAddress.h"

class ETXMetric: public EventHandler {
protected:
    enum MessageKind {
       SEND_PROBE,
    };

    struct MacNeighbour {
        IPv4Address neighbour;
        simtime_t newestUpdate;
        int x; //Especifica o valor de x informado pelo vizinho
        std::list<simtime_t> y; //Marca todas as recepções de pacotes
    };

    struct CandidateSet {
        IPv4Address target;
        Coord position;
        std::set<IPv4Address> forwardSet;
        bool isNeighbour;

        CandidateSet() : isNeighbour(false) {

        }
    };

    typedef std::map<IPv4Address, CandidateSet> CandidateSets;
    typedef std::map<IPv4Address, MacNeighbour> Neighbours;


    long packetsReceived;
    double meanValue;

    Neighbours neighbours;
    CandidateSets cSets;
    int probeSize; //Especifica o tamanho do pacote probe
    simtime_t probeTime; //Especifica a frequencia de envio de pacotes probes
    simtime_t etxW; //Representa o tamanho da janela
    cMessage* sendProbeEvent;
    simsignal_t rcvdPkSignal;
    simsignal_t sentPkSignal;

    virtual short int getModuleKind();
    virtual const char* getModuleName();
    cPacket* createProbe();
    double getExpectedPck() const;
    void removeOldEntries();
    void removeNeighbour(IPv4Address neighbour);//Remove todas as referencias a um vizinho
    void parseInitialConfig();
public:
    ETXMetric();
    virtual ~ETXMetric();
    virtual void processTimer(cMessage* msg);
    virtual void initialize();
    virtual void processNetPck(IPv4Address from, cMessage* msg);
    virtual void finish();
    double getCost(IPv4Address neighbour);//Retorna o cuso para o vizinho ou -1 caso ele nao esteja conectado
    std::list<IPv4Address> findCandidateSet(IPv4Address target);//Retorna o conjunto de candidatos baseados na localização geográfica dos nos
    IPv4Address findBestNeighbourTo(IPv4Address target);//Retorna o melhor dos vizinhos baseados na métrica etx
    double getTransmittionTime();

};

#endif /* ETXMETRIC_H_ */
