
#ifndef _HELPERS_
#define _HELPERS_

#define LOG_NODE(x, message) EV<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>[Node "<<x->myNetwAddr<<"] "<<message<<endl
#define LOG_PACKET(node, packet) EV<<"[From: "<<packet->getSrcAddr()<<" -- To: "<<packet->getDestAddr()<<"Recebido por: "<<node->myNetwAddr<<endl

#include <sstream>
#include <string>
#include <vector>
using namespace std;



template<typename T, typename K> string concat(T s, K s2) {
    stringstream ss;
    ss << s << s2;
    return ss.str();
}

template <typename T>
T convertToNumber ( const string &Text )//Text not by const reference so that the function can be used with a
{                               //character array as argument
    stringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

template<typename T> string convertToSrt(T s) {
    stringstream ss;
    ss << s;
    return ss.str();
}

void print(std::vector<uint8_t> v,uint32_t nodeId);




#endif //_HELPERS_
