/*
 * GaussJordan.hpp
 *
 *  Created on: Jan 27, 2013
 *      Author: ivocalado
 */

#ifndef GAUSSJORDAN_HPP_
#define GAUSSJORDAN_HPP_

#include <algorithm> // for std::swap
#include <cstddef>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "Binary8Arithmetic.h"
#include <vector>
using namespace std;
using boost::numeric::ublas::matrix;
using boost::numeric::ublas::zero_matrix;

namespace ncorp {

//Retorna a matrix transposta
matrix<uint8_t> transpose(matrix<uint8_t> const& m);

//Adiciona o vetor como uma nova linha da matriz
void add_row(matrix<uint8_t>& m, std::vector<uint8_t> const &v);

//Adiciona o vetor como uma nova coluna a tabela
void add_col(matrix<uint8_t>& m, std::vector<uint8_t> const &v);

//Converte a matriz para vetor. A matriz obrigatoriamente deve ser de linha
std::vector<uint8_t> convert_to_vector(matrix<uint8_t> const &m);

//Converte o vetor para matriz.
matrix<uint8_t> convert_to_matrix(std::vector<uint8_t> const &v);

////Retorna verdadeiro se matriz for nula
bool is_zero(matrix<uint8_t> const &m);

//Retorna verdadeiro se vetor for nulo
bool is_zero(std::vector<uint8_t> const &v);

//Calcula a dimensao da matriz passada ao mesmo tempo que a converte para RREF
size_t calculate_dimension(matrix<uint8_t> &m);

//Remove uma linha. Obrigatoriamente o indice tem de ser valido e a matrizes deve conter linhas
matrix<uint8_t> remove_row(matrix<uint8_t> const& m, size_t index);

//Concatena verticalmente a matrix b ao final da matrix a.
void concat_vertically(matrix<uint8_t>& a, matrix<uint8_t> const& b);

//Defines the interface of a matrix compliant with boots matrix interface
template<class Matrix> struct default_matrix_tr {
    typedef size_t index_type;
    typedef typename Matrix::value_type value_type;

    //Retorna a posicao inicial da linha
    static index_type min_row(Matrix const& A) {
        return 0;
    }

    //Retorna o indice da ultima linha
    static index_type max_row(Matrix const& A) {
        return A.size1() - 1;
    }

    //Retorna o indice da primeira coluna
    static index_type min_column(Matrix const& A) {
        return 0;
    }

    //Retorna o indice da ultima coluna
    static index_type max_column(Matrix const& A) {
        return A.size2() - 1;
    }

    static value_type& element(Matrix& A, index_type i, index_type k) {
        return A(i, k);
    }
    static value_type element(Matrix const& A, index_type i, index_type k) {
        return A(i, k);
    }

    //Cria a matrix com base no numero de linhas e colunas
    static Matrix create_matrix(index_type i, index_type j) {
        return zero_matrix<value_type>(i, j);
    }

    //Retorna a matriz transposta
    static Matrix trans(Matrix& A) {
        return boost::numeric::ublas::trans(A);
    }

};

template<class Type> struct default_arithmetic_traits {
    typedef Type value_type;

    static value_type zero() {
        cerr << "default_arithmetic_traits::zero()" << endl;
        return 0;
    }

    static value_type one() {
        cerr << "default_arithmetic_traits::one()" << endl;
        return 1;
    }

    static value_type add(value_type a, value_type b) {
        cerr << "default_arithmetic_traits::add()" << endl;
        return a + b;
    }

    static value_type sub(value_type a, value_type b) {
        cerr << "default_arithmetic_traits::sub()" << endl;
        return a - b;
    }

    static value_type mul(value_type a, value_type b) {
        cerr << "default_arithmetic_traits::mul()" << endl;
        return a * b;
    }

    static value_type div(value_type a, value_type b) {
        cerr << "default_arithmetic_traits::div()" << endl;
        return a / b;
    }

    static value_type negate(value_type a) {
        cerr << "default_arithmetic_traits::negate()" << endl;
        return -a;
    }

    static value_type reciprocal(value_type a) {
        cerr << "default_arithmetic_traits::reciprocal()" << endl;
        return 1 / a;
    }
};

template<class Matrix, class Traits> struct matrix_tr {
    typedef typename Traits::index_type index_type;
    typedef typename Traits::value_type value_type;
    static Traits traits;

    static Matrix create_matrix(index_type i, index_type j) {
        return traits.create_matrix(i, j);
    }
    static index_type min_row(Matrix const& A) {
        return traits.min_row(A);
    }
    static index_type max_row(Matrix const& A) {
        return traits.max_row(A);
    }
    static index_type min_column(Matrix const& A) {
        return traits.min_column(A);
    }
    static index_type max_column(Matrix const& A) {
        return traits.max_column(A);
    }
    static value_type& element(Matrix& A, index_type i, index_type k) {
        return traits.element(A, i, k);
    }
    static value_type element(Matrix const& A, index_type i, index_type k) {
        return traits.element(A, i, k);
    }
};

// Swap rows i and k of a matrix A
// Note that due to the reference, both dimensions are preserved for
// built-in arrays
template<class Matrix, class Traits = default_matrix_tr<Matrix> >
void swap_rows(Matrix& A, typename matrix_tr<Matrix, Traits>::index_type i,
        typename matrix_tr<Matrix, Traits>::index_type k) {
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;

    // check indices
    assert(mt.min_row(A) <= i);
    assert(i <= mt.max_row(A));

    assert(mt.min_row(A) <= k);
    assert(k <= mt.max_row(A));

    for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
        std::swap(mt.element(A, i, col), mt.element(A, k, col));
}

// divide row i of matrix A by v
template<class Matrix = matrix<uint8_t>,
        class Traits = default_matrix_tr<Matrix>,
        class ArithmeticTraits = binary8_arithmetic_traits>
void divide_row(Matrix& A, typename matrix_tr<Matrix, Traits>::index_type i,
        typename matrix_tr<Matrix, Traits>::value_type v) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;

    assert(mt.min_row(A) <= i);
    assert(i <= mt.max_row(A));

    assert(v != 0);

    for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
        mt.element(A, i, col) = ar.div(mt.element(A, i, col), v);
}

// Multiplies the specified row in this matrix by the specified factor. In other words, row *= factor.
template<class Matrix = matrix<uint8_t>,
        class Traits = default_matrix_tr<Matrix>,
        class ArithmeticTraits = binary8_arithmetic_traits>
void multiply_row(Matrix& A, typename matrix_tr<Matrix, Traits>::index_type i,
        typename matrix_tr<Matrix, Traits>::value_type v) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;

    assert(mt.min_row(A) <= i);
    assert(i <= mt.max_row(A));

    assert(v != 0);

    for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
        mt.element(A, i, col) = ar.mul(mt.element(A, i, col), v);
}

// Adds the first specified row in this matrix multiplied by the specified factor to the second specified row.
template<class Matrix = matrix<uint8_t>,
        class Traits = default_matrix_tr<Matrix>,
        class ArithmeticTraits = binary8_arithmetic_traits>
void add_rows(Matrix& A, typename matrix_tr<Matrix, Traits>::index_type src,
        typename matrix_tr<Matrix, Traits>::index_type dest,
        typename matrix_tr<Matrix, Traits>::value_type factor) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;

    assert(mt.min_row(A) <= src);
    assert(src <= mt.max_row(A));
    assert(mt.min_row(A) <= dest);
    assert(dest <= mt.max_row(A));

//    assert(factor != 0);

    for (index_type col = mt.min_column(A); col <= mt.max_column(A); ++col)
        mt.element(A, dest, col) = ar.add(mt.element(A, dest, col),
                ar.mul(mt.element(A, src, col), factor));
}

template<class Matrix = matrix<uint8_t>,
        class Traits = default_matrix_tr<Matrix>,
        class ArithmeticTraits = binary8_arithmetic_traits>
Matrix multiply(const Matrix& A, const Matrix& B) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;
    typedef typename matrix_tr<Matrix, Traits>::value_type value_type;

    assert(mt.max_column(A) == mt.max_row(B));

    index_type rows = mt.max_row(A) + 1;
    index_type cols = mt.max_column(B) + 1;
    index_type cells = mt.max_column(A) + 1;

    Matrix result = mt.create_matrix(rows, cols);

    for (index_type i = 0; i < rows; i++) {
        for (index_type j = 0; j < cols; j++) {
            value_type sum = ar.zero();
            for (index_type k = 0; k < cells; k++)
                sum = ar.add(ar.mul(mt.element(A, i, k), mt.element(B, k, j)),
                        sum);
            mt.element(result, i, j) = sum;
        }
    }
    return result;
}

template<typename Matrix = matrix<uint8_t>, class Traits = default_matrix_tr<
        Matrix>, class ArithmeticTraits = binary8_arithmetic_traits>
void to_reduced_row_echelon_form(Matrix& A) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;
    typedef typename matrix_tr<Matrix, Traits>::value_type value_type;

    index_type rows = mt.max_row(A) + 1;
    index_type cols = mt.max_column(A) + 1;

    // Compute row echelon form (REF)
    index_type numPivots = 0;
    for (index_type j = 0; j < cols; j++) {  // For each column
        // Find a pivot row for this column
        index_type pivotRow = numPivots;
        while (pivotRow < rows && (mt.element(A, pivotRow, j) == ar.zero())) {
            pivotRow++;
        }
        if (pivotRow == rows)
            continue;  // Cannot eliminate on this column
        swap_rows<Matrix, Traits>(A, numPivots, pivotRow);
        pivotRow = numPivots;
        numPivots++;
        // Simplify the pivot row
        multiply_row<Matrix, Traits, ArithmeticTraits>(A, pivotRow,
                ar.reciprocal(mt.element(A, pivotRow, j)));
        // Eliminate rows below
        for (index_type i = pivotRow + 1; i < rows; i++) {
            add_rows<Matrix, Traits, ArithmeticTraits>(A, pivotRow, i,
                    ar.negate(mt.element(A, i, j)));
        }
    }
    // Compute reduced row echelon form (RREF)
    for (int i = rows - 1; i >= 0; i--) {
        // Find pivot
        index_type pivotCol = 0;
        while (pivotCol < cols && (mt.element(A, i, pivotCol) == ar.zero())) {
            pivotCol++;
        }

        if (pivotCol == cols)
            continue;  // Skip this all-zero row
        // Eliminate rows above
        for (int j = i - 1; j >= 0; --j) {
            add_rows<Matrix, Traits, ArithmeticTraits>(A, i, j,
                    ar.negate(mt.element(A, j, pivotCol)));
        }
    }
}

//Replaces the values of this matrix with the inverse of this matrix. Requires the matrix to be square
template<typename Matrix = matrix<uint8_t>, class Traits = default_matrix_tr<
        Matrix>, class ArithmeticTraits = binary8_arithmetic_traits>
void invert(Matrix& A) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;
    typedef typename matrix_tr<Matrix, Traits>::value_type value_type;

    index_type rows = mt.max_row(A) + 1;
    index_type cols = mt.max_column(A) + 1;

    assert(rows == cols);

    Matrix temp = mt.create_matrix(rows, cols * 2);

    for (index_type i = 0; i < rows; i++) {
        for (index_type j = 0; j < cols; j++) {
            mt.element(temp, i, j) = mt.element(A, i, j);
            mt.element(temp, i, j + cols) = i == j ? ar.one() : ar.zero();
        }
    }

    to_reduced_row_echelon_form(temp);

    // Check that the RREF is in this form: [identity | inverse]
    for (index_type i = 0; i < rows; i++) {
        for (index_type j = 0; j < cols; j++) {
            //If fails the matrix is not invertible
            assert(mt.element(temp, i, j) == (i == j) ? ar.one() : ar.zero());
        }
    }

    // Extract inverse matrix
    for (index_type i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            mt.element(A, i, j) = mt.element(temp, i, j + cols);
    }
}

//Replaces the values of this matrix with the inverse of this matrix. Requires the matrix to be square
template<typename Matrix = matrix<uint8_t>, class Traits = default_matrix_tr<
        Matrix>, class ArithmeticTraits = binary8_arithmetic_traits> typename matrix_tr<
        Matrix, Traits>::value_type determinantAndRef(Matrix& A) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;
    typedef typename matrix_tr<Matrix, Traits>::value_type value_type;

    index_type rows = mt.max_row(A) + 1;
    index_type cols = mt.max_column(A) + 1;

    //If fails matrix dimensions are not square
    assert(rows == cols);

    value_type det = ar.one();

    // Compute row echelon form (REF)
    index_type numPivots = 0;
    for (index_type j = 0; j < cols; j++) {  // For each column
        // Find a pivot row for this column
        index_type pivotRow = numPivots;
        while (pivotRow < rows && mt.element(A, pivotRow, j) == ar.zero())
            pivotRow++;

        if (pivotRow < rows) {
            // This column has a nonzero pivot
            if (numPivots != pivotRow) {
                swap_rows<Matrix, Traits>(A, numPivots, pivotRow);
                det = ar.negate(det);
            }
            pivotRow = numPivots;
            numPivots++;

            // Simplify the pivot row
            value_type temp = mt.element(A, pivotRow, j);
            multiply_row<Matrix, Traits, ArithmeticTraits>(A, pivotRow,
                    ar.reciprocal(temp));
            det = ar.multiply(temp, det);

            // Eliminate rows below
            for (index_type i = pivotRow + 1; i < rows; i++)
                add_rows<Matrix, Traits, ArithmeticTraits>(A, pivotRow, i,
                        ar.negate(mt.element(A, i, j)));
        }

        // Update determinant
        det = ar.multiply(mt.element(A, j, j), det);
    }

    return det;
}

//Returns the rank of the matrix
template<typename Matrix = matrix<uint8_t>, class Traits = default_matrix_tr<
        Matrix>, class ArithmeticTraits = binary8_arithmetic_traits> size_t rank(
        Matrix const& A) {
    ArithmeticTraits ar;
    matrix_tr<Matrix, Traits> mt;
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;
    typedef typename matrix_tr<Matrix, Traits>::value_type value_type;

    Matrix m(A);
    to_reduced_row_echelon_form<Matrix, Traits, ArithmeticTraits>(m);

    size_t counter = 0;
    for (index_type i = 0; i < m.size1(); i++) {
        for (index_type j = mt.min_column(m); j <= mt.max_column(m); j++) {
            if (mt.element(m, i, j) != ar.zero()) {
                counter++;
                break;
            }
        }
    }
    return counter;
}

//Returns true if the vector is LI compared to the Matrix (for now, we are considering the vector compliant with vector<T>
template<typename Type = uint8_t, typename Matrix = matrix<Type>,
        class Traits = default_matrix_tr<Matrix>,
        class ArithmeticTraits = binary8_arithmetic_traits,
        typename Vector = std::vector<Type> > bool is_LI(Matrix const& A,
        Vector v) {
    typedef typename matrix_tr<Matrix, Traits>::index_type index_type;
    typedef typename matrix_tr<Matrix, Traits>::value_type value_type;

    Matrix m(A);
    size_t rank1 = rank(m);
    add_row(m, v);
    size_t rank2 = rank(m);
    return rank1 != rank2;
}

} // namespace ncorp
#endif /* GAUSSJORDAN_HPP_ */
