/*
 * HashMatrices.cpp
 *
 *  Created on: Jan 25, 2013
 *      Author: ivocalado
 */

#include "HashMatrices.h"
#include <cstdlib>
HashMatrices::HashMatrices(size_t numberOfMatrices, size_t matricesSize, long seed) {
    for(unsigned int i = 0; i < numberOfMatrices; i++) {
        DiagonalRandomMatrix m(matricesSize, seed);
        matrices.push_back(m);
        seed = rand();
    }
}

const matrix<uint8_t>& HashMatrices::getMatrix(int index) const {
    return matrices[index].getMatrix();
}

HashMatrices::~HashMatrices() {
    // TODO Auto-generated destructor stub
}

