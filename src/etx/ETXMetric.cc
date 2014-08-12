/*
 * ETXMetric.cc
 *
 *  Created on: Jun 3, 2014
 *      Author: ivocalado
 */

#include "ETXMetric.h"
#include "packets_m.h"
#include <algorithm>
#include <utility>
#include "helper-functions.h"
#include <fstream>
#include <iostream>

using namespace std;

ETXMetric::ETXMetric() {
    sendProbeEvent = NULL;
}

ETXMetric::~ETXMetric() {
    // TODO Auto-generated destructor stub
}

short int ETXMetric::getModuleKind() {
    return ETX_MODULE;
}

const char* ETXMetric::getModuleName() {
    return "pkt-etx-probe";
}

void ETXMetric::initialize() {
    EventHandler::initialize();
    neighbours.clear();

    probeSize = par("ETXProbeSize");
    probeTime = par("ETXProbeTime");
    etxW = par("ETXW");

    if (!par("ETXDisableEvents").boolValue()) {
        sendProbeEvent = new cMessage("etx-send-probe", SEND_PROBE);
        scheduleTimer(probeTime + dblrand(), sendProbeEvent);

        sentPkSignal = registerSignal("ETXSentPk");
        rcvdPkSignal = registerSignal("ETXRcvdPk");
    }

    if (par("ETXUseConfigFile")) {
        parseInitialConfig();
    }

    packetsReceived = 0;
    meanValue = 0;
}

void ETXMetric::processTimer(cMessage* msg) {

    switch (msg->getKind()) {
    case SEND_PROBE: {
        auto pck = createProbe();
        emit(sentPkSignal, pck);
        sendDown(pck, IPv4Address::ALLONES_ADDRESS);
        scheduleTimer(probeTime, msg);
    }
        break;
    default:
        break;
    }
}

cPacket* ETXMetric::createProbe() {
    removeOldEntries();

    auto probe = new EtxProbe();
    probe->setNeighboursArraySize(neighbours.size());
    probe->setRecPacketsArraySize(neighbours.size());
    probe->setBitLength(probeSize);
    probe->setSendTime(simTime());

    int counter = 0;
    std::for_each(neighbours.begin(), neighbours.end(),
            [&counter, probe](std::pair<const IPv4Address, MacNeighbour>& p) {
                probe->setNeighbours(counter, p.first);
                probe->setRecPackets(counter, p.second.y.size());
                counter++;
            });

    probe->setPosition(getMyPosition());
    std::list<Target> forwardSet;

    auto myNetAddr = getMyNetAddr();
    std::for_each(cSets.begin(), cSets.end(),
            [&forwardSet, myNetAddr](std::pair<const IPv4Address, CandidateSet>& p) {
                Target t;
                t.addr = p.first;
                t.position = p.second.position;
                forwardSet.push_back(t);
            });
    probe->setForwardSet(forwardSet);

    return probe;
}

void ETXMetric::processNetPck(IPv4Address from, cMessage* msg) {
    auto pkt = check_and_cast<NcorpPacket*>(msg);
    switch (pkt->getType()) {
    case ETX_PROBE: {
        auto now = simTime();
        auto myNetAddr = getMyNetAddr();
        auto pck = dynamic_cast<EtxProbe*>(msg);

        packetsReceived++;
        meanValue += (now.dbl() - pck->getSendTime().dbl());

        emit(rcvdPkSignal, pck);
        ASSERT(pck->getNeighboursArraySize() == pck->getRecPacketsArraySize());

        neighbours[from].neighbour = from;
        neighbours[from].newestUpdate = now;
        neighbours[from].y.push_back(now);
        if (neighbours[from].y.size() > getExpectedPck()) //Se o numero de pacote for maior que o necessario remove o mais antigo
            neighbours[from].y.pop_front();
        for (auto it = 0U; it < pck->getNeighboursArraySize(); it++) {
            if (myNetAddr == pck->getNeighbours(it)) {
                neighbours[from].x = pck->getRecPackets(it);
                break;
            }
        }

        /**
         * Atualiza a posição do no vizinho
         */
        cSets[from].target = from;
        cSets[from].isNeighbour = true;
        auto neighbourPosition = pck->getPosition();
        cSets[from].position = neighbourPosition;

        /**
         * Para cada report:
         *  1 Recupera a posição do destino
         *  2 Verifica se o vizinho é mais próximo do destino que o nó corrente
         *      2.1 Se for, adiciona o vizinho ao fs do destino
         *      2.2 Se não, verifica se o vizinho já está no fs. Em caso positivo, remove o vizinho do fs (indica que o vizinho nao consegue mais entregar dados)
         *          2.2.1 Se o target não for vizinho direto e nem possuir fs, remove ele
         */
        for_each(pck->getForwardSet().begin(), pck->getForwardSet().end(),
                [this, neighbourPosition, from](Target t) {
                    auto targetPosition = t.position;
                    auto currentPosition = getMyPosition();
                    if(t.addr != getMyNetAddr()) {
                        if(neighbourPosition.distance(targetPosition) < currentPosition.distance(targetPosition)) {
                            cSets[t.addr].forwardSet.insert(from);
                            cSets[t.addr].position = targetPosition;
                            cSets[t.addr].target = t.addr;
                        } /*else {
                         auto &fs = cSets[t.addr].forwardSet;
                         if(std::find(fs.begin(), fs.end(), from) != fs.end()) { //
                         fs.erase(from);
                         if(!cSets[t.addr].isNeighbour && fs.empty()) {
                         cSets.erase(t.addr);

                         }
                         }
                         }*/
                    }

                });
    }
        delete msg;

        break;
    default: {
        throw new cRuntimeError("Pacote invalido encaminhado ao modulo ETX");
    }

    }
}

void ETXMetric::finish() {

    if (sendProbeEvent)
        delete cancelEvent(sendProbeEvent);

    if (par("ETXOutputConfig").boolValue()) {

        cXMLElement* root = new cXMLElement("node", "", NULL);
        root->setAttribute("id", convertToSrt(getMyNetAddr()).c_str());
        auto accumulated_difference = new cXMLElement("accumulated_difference",
                "", root);
        auto mValue = convertToSrt(meanValue);
        accumulated_difference->setNodeValue(mValue.c_str(), mValue.length());

        auto num_packets_received = new cXMLElement("num_packets_received", "",
                root);
        auto num_packets_received_str = convertToSrt(packetsReceived);
        num_packets_received->setNodeValue(num_packets_received_str.c_str(),
                num_packets_received_str.length());

        auto transmitionTime = new cXMLElement("transmition_time", "", root);
        auto transmitionTimeStr = convertToSrt(getTransmittionTime());
        transmitionTime->setNodeValue(transmitionTimeStr.c_str(),
                transmitionTimeStr.length());

        auto localTable = new cXMLElement("local_table", "", root);
        for_each(neighbours.begin(), neighbours.end(),
                [localTable, this](std::pair<const IPv4Address, MacNeighbour>& p) {
                    auto neighbour = new cXMLElement("neighbour", "", localTable);
                    neighbour->setAttribute("id", convertToSrt(p.second.neighbour).c_str());
                    auto x = new cXMLElement("dx", "", neighbour);
                    auto xStr = convertToSrt(p.second.x);
                    x->setNodeValue(xStr.c_str(), xStr.length());

                    auto y = new cXMLElement("dy", "", neighbour);
                    auto yStr = convertToSrt(p.second.y.size());
                    y->setNodeValue(yStr.c_str(), yStr.length());

                    auto cost = new cXMLElement("cost", "", neighbour);
                    auto costStr = convertToSrt(getCost(p.first));
                    cost->setNodeValue(costStr.c_str(), costStr.size());
                });

        auto globalTable = new cXMLElement("global_table", "", root);
        for_each(cSets.begin(), cSets.end(),
                [globalTable, this](std::pair<const IPv4Address, CandidateSet> & p) {
                    auto target = new cXMLElement("target", "", globalTable);

                    auto targetAddr = new cXMLElement("addr", "", target);
                    auto targetStr = convertToSrt(p.first);
                    targetAddr->setNodeValue(targetStr.c_str(), targetStr.length());

                    auto isNeighbour = new cXMLElement("is_neighbour", "", target);
                    auto isNeighbourStr = convertToSrt(p.second.isNeighbour);
                    isNeighbour->setNodeValue(isNeighbourStr.c_str(), isNeighbourStr.length());

                    auto position = new cXMLElement("position", "", target);

                    auto x = new cXMLElement("x", "", position);
                    auto xStr = convertToSrt(p.second.position.x);
                    x->setNodeValue(xStr.c_str(), xStr.length());

                    auto y = new cXMLElement("y", "", position);
                    auto yStr = convertToSrt(p.second.position.y);
                    y->setNodeValue(yStr.c_str(), yStr.length());

                    auto z = new cXMLElement("z", "", position);
                    auto zStr = convertToSrt(p.second.position.z);
                    z->setNodeValue(zStr.c_str(), zStr.length());

                    auto distance = new cXMLElement("distance", "", position);
                    auto distanceStr = convertToSrt(getMyPosition().distance(p.second.position));
                    distance->setNodeValue(distanceStr.c_str(), distanceStr.length());

                    auto fs = new cXMLElement("forward_set", "", target);
                    for_each(p.second.forwardSet.begin(), p.second.forwardSet.end(), [fs](IPv4Address addr) {
                                auto neighbour = new cXMLElement("neighbour", "", fs);
                                auto neighbourStr = convertToSrt(addr);
                                neighbour->setNodeValue(neighbourStr.c_str(), neighbourStr.length());
                            });
                });

        std::ofstream out(par("ETXOutputFileName").stringValue(),
                std::ios::app);
        if (out.is_open()) {
            out << root->detailedInfo() << endl;
        } else {
            throw new cRuntimeError(
                    "Erro ao abrir arquivo de saida de configuração ETX");
        }
    }
}

double ETXMetric::getCost(IPv4Address neighbour) {

    auto it = neighbours.find(neighbour);
    if (it == neighbours.end())
        return -1;

    double expectedPk = getExpectedPck();
    double pkRec = it->second.y.size();
    double pr = pkRec / expectedPk;
    double ps = it->second.x / expectedPk;

    if (pr > 1)
        pr = 1;
    if (ps > 1)
        ps = 1;
    auto result = (ps == 0 || pr == 0) ? 1e100 : 1 / (ps * pr);
    return result;
}

double ETXMetric::getExpectedPck() const {
    return etxW / probeTime;
}

void ETXMetric::removeOldEntries() {
    auto now = simTime();
    auto func = [now, this](simtime_t event) {
        return (now - event) > this->etxW;
    };

    //Se
    for (auto it = neighbours.begin(); it != neighbours.end(); it++) {
        if (func(it->second.newestUpdate)) {
            removeNeighbour(it->first);
        } else {
            it->second.y.remove_if(func);
            if (it->second.y.empty())
                removeNeighbour(it->first);
        }
    }
}

void ETXMetric::removeNeighbour(IPv4Address neighbour) {
    neighbours.erase(neighbour); //Remove o vizinho da tabela local

    //Remove todas as referencias ao vizinho na tabela de candidatos
    cSets.erase(neighbour);

    std::set<IPv4Address> keysToRemove;
    for_each(cSets.begin(), cSets.end(),
            [&keysToRemove, neighbour](std::pair<const IPv4Address, CandidateSet>& c) {
                c.second.forwardSet.erase(neighbour);
                if(!c.second.isNeighbour && c.second.forwardSet.empty())
                keysToRemove.insert(c.first);
            });

    for_each(keysToRemove.begin(), keysToRemove.end(),
            [this](IPv4Address addr) {
                cSets.erase(addr);
            });
}

void ETXMetric::parseInitialConfig() {
    auto root = par("ETXInputFileName").xmlValue()->getFirstChildWithAttribute(
            "node", "id", convertToSrt(getMyNetAddr()).c_str());
    if (!root) {
        opp_error(
                "Arquivo de configuração inválido. Nem todos os nós estão sendo contemplados");
    }

    meanValue =
            convertToNumber<double>(
                    root->getFirstChildWithTag("accumulated_difference")->getNodeValue());
    packetsReceived = convertToNumber<long>(
            root->getFirstChildWithTag("num_packets_received")->getNodeValue());

    auto localTable =
            root->getFirstChildWithTag("local_table")->getChildrenByTagName(
                    "neighbour");
    for (auto it = localTable.begin(); it != localTable.end(); it++) {
        auto neighbour = IPv4Address((*it)->getAttribute("id"));
        auto dx = convertToNumber<int>(
                (*it)->getFirstChildWithTag("dx")->getNodeValue());
        auto dy = convertToNumber<int>(
                (*it)->getFirstChildWithTag("dy")->getNodeValue());
        neighbours[neighbour].neighbour = neighbour;
        neighbours[neighbour].x = dx;
        neighbours[neighbour].newestUpdate = simTime();
        for (int i = 0; i < dy; i++) {
            neighbours[neighbour].y.push_back(simTime());
        }
    }
    auto globalTable =
            root->getFirstChildWithTag("global_table")->getChildrenByTagName(
                    "target");
    for (auto it = globalTable.begin(); it != globalTable.end(); it++) {
        auto addr = IPv4Address(
                (*it)->getFirstChildWithTag("addr")->getNodeValue());
        auto isNeighour = convertToNumber<bool>(
                (*it)->getFirstChildWithTag("is_neighbour")->getNodeValue());
        auto x = convertToNumber<double>(
                (*it)->getFirstChildWithTag("position")->getFirstChildWithTag(
                        "x")->getNodeValue());
        auto y = convertToNumber<double>(
                (*it)->getFirstChildWithTag("position")->getFirstChildWithTag(
                        "y")->getNodeValue());
        auto z = convertToNumber<double>(
                (*it)->getFirstChildWithTag("position")->getFirstChildWithTag(
                        "z")->getNodeValue());
        std::set<IPv4Address> forwardSet;
        auto fs =
                (*it)->getFirstChildWithTag("forward_set")->getChildrenByTagName(
                        "neighbour");
        for (auto it2 = fs.begin(); it2 != fs.end(); it2++) {
            forwardSet.insert(IPv4Address((*it2)->getNodeValue()));
        }
        cSets[addr].target = addr;
        cSets[addr].position = Coord(x, y, z);
        cSets[addr].isNeighbour = isNeighour;
        cSets[addr].forwardSet = forwardSet;
    }
}

std::list<IPv4Address> ETXMetric::findCandidateSet(IPv4Address target) {
    std::list<IPv4Address> result;
    auto candidateSet = cSets.find(target);
    if (candidateSet != cSets.end()) {
        std::vector<std::pair<IPv4Address, Coord> > tmpfs;
        for_each(candidateSet->second.forwardSet.begin(),
                candidateSet->second.forwardSet.end(),
                [&tmpfs, this](IPv4Address addr) {
                    tmpfs.push_back(make_pair(addr, cSets[addr].position));
                });

        auto targetPosition = cSets[target].position;
        std::sort(tmpfs.begin(), tmpfs.end(),
                [targetPosition, this](std::pair<IPv4Address, Coord> node1, std::pair<IPv4Address, Coord> node2) {
                    return targetPosition.distance(node1.second) < targetPosition.distance(node2.second);
                });

        std::for_each(tmpfs.begin(), tmpfs.end(),
                [&result](std::pair<IPv4Address, Coord> node1) {
                    result.push_back(node1.first);
                });
    }

    return result;
}

IPv4Address ETXMetric::findBestNeighbourTo(IPv4Address target) {
    if (cSets.find(target) == cSets.end())
        return IPv4Address::UNSPECIFIED_ADDRESS;
    auto fs = cSets[target].forwardSet;
    std::map<IPv4Address, double> m;

    for_each(fs.begin(), fs.end(), [&m, this](IPv4Address adr) {
        m[adr] = getCost(adr);
    });

    if (cSets[target].isNeighbour)
        m[target] = getCost(target);

    ASSERT(!m.empty());

    return std::min_element(m.begin(), m.end(),
            [](std::pair<const IPv4Address, double>& p1, std::pair<const IPv4Address, double>& p2) {return p1.second < p2.second;})->first;
}

double ETXMetric::getTransmittionTime() {
    return packetsReceived ? meanValue / packetsReceived : 0;
}
