/*
 * HashMatrices.h
 *
 *  Created on: Jan 25, 2013
 *      Author: ivocalado
 */

#ifndef HASHMATRICES_H_
#define HASHMATRICES_H_
#include "DiagonalRandomMatrix.h"
#include <vector>
#include <algorithm>
class HashMatrices {
    std::vector<DiagonalRandomMatrix> matrices;
public:
    template<typename T> friend T& operator<<(T &out, HashMatrices &m);
    HashMatrices(size_t numberOfMatrices, size_t matricesSize, long nodeId);
    const matrix<uint8_t>& getMatrix(int index) const;
    virtual ~HashMatrices();
};

template<typename T> T& operator<<(T &out, HashMatrices &m) {
    std::for_each(m.matrices.begin(), m.matrices.end(), [&out](DiagonalRandomMatrix diagonal){
        out<<diagonal<<"\n"<<std::endl;
    });
    return out;
}

#endif /* HASHMATRICES_H_ */
