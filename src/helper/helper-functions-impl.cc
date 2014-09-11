/*
 * helper-functions-impl.cc
 *
 *  Created on: Jul 12, 2014
 *      Author: ivocalado
 */

#include "helper-functions.h"
#include <stdio.h>
#include <stdarg.h>
#include "constants.h"

void print(std::vector<uint8_t> v,uint32_t nodeId) {
    stringstream st;
    st<<"(";
    for (size_t i = 0; i < v.size(); i++) {
        st << static_cast<int>(v[i]) << ((i == (v.size() - 1)) ? "" : " ");
    }
    st << ")";
    fprintf(stderr, "[Node = %d] %s\n", nodeId, st.str().c_str());
}

int debugprintf(FILE* out, LogLevel level, const char* format, ...) {

    if(CURRENT_LOG_LEVEL && level <= CURRENT_LOG_LEVEL) {
        va_list list;
        va_start(list, format);
        int result = vfprintf(out, format, list);
        va_end(list);
        return result;
    } else
        return 0;
}
