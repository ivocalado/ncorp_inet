/*
 * Binary8MatrixHelpers.cc
 *
 *  Created on: Feb 3, 2014
 *      Author: ivocalado
 */

#include "Binary8MatrixHelpers.h"

namespace ncorp {
//Retorna a matrix transposta
matrix<uint8_t> transpose(matrix<uint8_t> const& m) {
    return trans(m);
}

//Adiciona o vetor como uma nova linha da matriz
void add_row(matrix<uint8_t>& m, std::vector<uint8_t> const &v) {
    assert(m.size2() == v.size());

    m.resize(m.size1() + 1, m.size2(), true);
    size_t row = m.size1() - 1;

    for (size_t i = 0; i < v.size(); i++)
        m(row, i) = v[i];
}

size_t calculate_dimension(matrix<uint8_t> &m) {
    to_reduced_row_echelon_form(m);
    size_t counter = 0;
    for(size_t i = 0; i < m.size1(); i++) {
        bool isValid = false;
        for(size_t j = 0; j < m.size2(); j++) {
            if(m(i, j)) {
                isValid = true;
                break;
            }
        }

        counter += isValid;
    }
    return counter;
}

//Converte o vetor para matriz.
matrix<uint8_t> convert_to_matrix(std::vector<uint8_t> const &v) {
    assert(!v.empty());
    matrix<uint8_t> m(1, v.size());

    for (size_t i = 0; i < v.size(); i++)
        m(0, i) = v[i];
    return m;
}

////Retorna verdadeiro se matriz for nula
bool is_zero(matrix<uint8_t> const &m) {
    for (size_t i = 0; i < m.size1(); i++)
        for (size_t j = 0; j < m.size2(); j++)
            if (m(i, j))
                return false;
    return true;
}

//Concatena verticalmente a matrix b ao final da matrix a.
void concat_vertically(matrix<uint8_t>& a, matrix<uint8_t> const& b) {
    assert(a.size2() == b.size2()); //Ambas as matrizes devem ter obrigatoriamente o mesmo numero de colunas
    size_t oldTop = a.size1();
    a.resize(a.size1() + b.size1(), a.size2(), true);
    for (size_t i = 0; i < b.size1(); i++, oldTop++) {
        for (size_t j = 0; j < b.size2(); j++) {
            a(oldTop, j) = b(i, j);
        }
    }
}

}  // namespace ncorp
