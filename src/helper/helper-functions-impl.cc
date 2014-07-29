/*
 * helper-functions-impl.cc
 *
 *  Created on: Jul 12, 2014
 *      Author: ivocalado
 */

#include "helper-functions.h"


void print(std::vector<uint8_t> v,uint32_t nodeId) {
    stringstream st;
    st<<"(";
    for (size_t i = 0; i < v.size(); i++) {
        st << static_cast<int>(v[i]) << ((i == (v.size() - 1)) ? "" : " ");
    }
    st << ")";
    fprintf(stderr, "[Node = %d] %s\n", nodeId, st.str().c_str());
}
