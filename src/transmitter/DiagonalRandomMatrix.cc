/*
 * EspecializedMatrix.cpp
 *
 *  Created on: Jan 25, 2013
 *      Author: ivocalado
 */

#include "DiagonalRandomMatrix.h"
#include <stdlib.h>
#include <iostream>
using namespace std;


using namespace boost::numeric::ublas;
DiagonalRandomMatrix::DiagonalRandomMatrix(uint8_t size, long seed) : m(zero_matrix<uint8_t>(size, size)) {
    srand(seed);
    for(unsigned int i = 0; i < size; i++) {
        m(i, i) = (rand() % 255)+1;
    }
}

const matrix<uint8_t>& DiagonalRandomMatrix::getMatrix() const {
    return m;
};

DiagonalRandomMatrix::~DiagonalRandomMatrix() {
    // TODO Auto-generated destructor stub
}

