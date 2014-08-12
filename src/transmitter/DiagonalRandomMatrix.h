/*
 * DiagonalRandomMatrix.h
 *
 *  Created on: Jan 25, 2013
 *      Author: ivocalado
 */

#ifndef DIAGONALRANDOMMATRIX_H_
#define DIAGONALRANDOMMATRIX_H_

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>

using namespace boost::numeric::ublas;
class DiagonalRandomMatrix {
    matrix<uint8_t> m;
public:
    const matrix<uint8_t>& getMatrix() const;
    DiagonalRandomMatrix(uint8_t size = 32, long seed = 1);
    virtual ~DiagonalRandomMatrix();
};

template<typename T> T& operator<<(T &out, DiagonalRandomMatrix &m) {
    const matrix<uint8_t>& mt = m.getMatrix();

    for (unsigned i = 0; i < mt.size1(); ++i) {
        out<<"(";
        for (unsigned j = 0; j < mt.size2(); ++j)
            out << static_cast<int>(mt(i, j)) << ((j == (mt.size2()-1))? ")":", ");
        out<<std::endl;
    }
    return out;
}

#endif /* ESPECIALIZEDMATRIX_H_ */
