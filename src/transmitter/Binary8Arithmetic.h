/*
 * Binary8Arithmetic.h
 *
 *  Created on: Jan 26, 2014
 *      Author: ivocalado
 */

#ifndef BINARY8ARITHMETIC_H_
#define BINARY8ARITHMETIC_H_
#include <iostream>
using namespace std;
#include "fifi/arithmetics.hpp"
#include "fifi/field_types.hpp"

namespace ncorp {

struct binary8_arithmetic_traits {
    typedef uint8_t value_type;
    fifi::log_table<fifi::binary8> ar;

    value_type zero() {
        return 0;
    }

    static value_type one() {
        return 1;
    }

    value_type add(value_type a, value_type b) {
        return ar.add(a, b);
    }

    value_type sub(value_type a, value_type b) {
        return ar.subtract(a, b);
    }

    value_type mul(value_type a, value_type b) {
        return ar.multiply(a, b);
    }

    value_type div(value_type a, value_type b) {
        return ar.divide(a, b);
    }

    value_type negate(value_type a) {

        return ar.subtract(0, a);
    }

    value_type reciprocal(value_type in) {
        /* 0 is self inverting */
        return ar.invert(in);
    }
};

}  // namespace ncorp


#endif /* BINARY8ARITHMETIC_H_ */
