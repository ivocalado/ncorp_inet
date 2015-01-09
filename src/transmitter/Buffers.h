/*
 * Buffers.h
 *
 *  Created on: Mar 18, 2013
 *      Author: ivocalado
 */

#ifndef BUFFERS_H_
#define BUFFERS_H_

#include <boost/numeric/ublas/matrix.hpp>

#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <deque>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "Binary8MatrixHelpers.h"
using namespace boost::numeric::ublas;
using namespace std;
namespace ncorp {
class Btx {
    size_t generation;
    matrix<uint8_t> m;
    std::deque<bool> h;
public:
    Btx(size_t g = 32) :
            generation(g), m(0, generation) {

    }

    void addRow(std::vector<uint8_t> row) {
        assert(row.size() == generation);

        ncorp::add_row(m, row);

        h.push_back(false);
    }

    std::vector<uint8_t> getRow(int index) {
        std::vector<uint8_t> result;
        matrix_row<matrix<uint8_t> > row(m, index);
        std::copy(row.begin(), row.end(), back_inserter(result));
        return result;
    }

    size_t countRows() {
        return h.size();
    }

    /**
     * Marca a linha como lida
     */
    void markRowAsHeard(unsigned int index) {
        assert(index >= 0 && index < h.size());
        h[index] = true;
    }

    /**
     * Retorna true se a linha já tiver sido lida
     */
    bool isRowHeard(unsigned int index) {
        assert(index >= 0 && index < h.size());
        return h[index];
    }

    /**
     * Retorna o número de linhas marcadas como ouvidas
     */
    int countRowsHeard() {
        int count = 0;
        std::for_each(h.begin(), h.end(), [&count](bool value) {
            if(value)
            count++;
        });
        return count;
    }

    /**
     * Recupera a matriz contendo apenas as linhas de brx com a flag h marcada
     */
    matrix<uint8_t> getHmatrix() {
        matrix<uint8_t> result(zero_matrix<uint8_t>(0, generation));
        for (size_t i = 0; i < m.size1(); i++) {
            if (h[i]) {
                add_row(result, getRow(i));
            }
        }
        return result;
    }

    void printInfo() {
        cerr << "Status (linhas marcadas/linhas totais) = " << countRowsHeard()
                << "/" << countRows() << endl;
    }
};

class Brx {
    size_t generation;
    matrix<uint8_t> m;
    std::deque<bool> h;
    std::vector<int> counter;
public:
    Brx(size_t g = 32) :
            generation(g), m(0, generation) {
    }

    void addRow(std::vector<uint8_t> row) {
        assert(row.size() == generation);

        ncorp::add_row(m, row);

        h.push_back(false);
        counter.push_back(0);
    }

    std::vector<uint8_t> getRow(int index) {
        std::vector<uint8_t> result;
        matrix_row<matrix<uint8_t> > row(m, index);
        std::copy(row.begin(), row.end(), back_inserter(result));
        return result;
    }

    /**
     * Marca a linha como lida
     */
    void markRowAsHeard(unsigned int index) {
        assert(index >= 0 && index < h.size());
        h[index] = true;
    }

    size_t countRows() {
        return h.size();
    }

    /**
     * Retorna o número de linhas marcadas como ouvidas
     */
    int countRowsHeard() {
        int count = 0;
        std::for_each(h.begin(), h.end(), [&count](bool value) {
            if(value)
            count++;
        });
        return count;
    }

    /**
     * Retorna true se a linha já tiver sido lida
     */
    bool isRowHeard(unsigned int index) {
        assert(index >= 0 && index < h.size());
        return h[index];
    }

    /**
     * Retorna o número de vezes que a linha já foi visitada
     */
    int rowCounter(unsigned int index) {
        assert(index >= 0 && index < counter.size());
        return counter[index];
    }

    /**
     * Incrementa o contador da linha
     */
    void incrementRowCounter(unsigned int index) {
        assert(index >= 0 && index < counter.size());
        counter[index]++;
    }

    /**
     * Recupera o indice para a linha com a menor contagem
     */
    int getLowestCountedRow() {
        assert(!counter.empty());

        long flag = counter[0];
        size_t index = 0;

        //Recupera o valor com a menor contagem
        for (unsigned int i = 0; i < counter.size(); i++) {
            if (counter[i] < flag) {
                flag = counter[i];
                index = i;
            }
        }
        return index;
    }

    /**
     * Recupera a matriz contendo apenas as linhas de brx com a flag h marcada
     */
    matrix<uint8_t> getHmatrix() {
        matrix<uint8_t> result(zero_matrix<uint8_t>(0, generation));
        for (size_t i = 0; i < m.size1(); i++) {
            if (h[i]) {
                add_row(result, getRow(i));
            }
        }
        return result;
    }

    /**
     * Recupera a matriz contendo apenas as linhas de brx com a flag h desmarcada
     */
    matrix<uint8_t> getNotHmatrix() {
        matrix<uint8_t> result(zero_matrix<uint8_t>(0, generation));
        for (size_t i = 0; i < m.size1(); i++) {
            if (!h[i]) {
                add_row(result, getRow(i));
            }
        }
        return result;
    }

    matrix<uint8_t> getMatrix() {
        return m;
    }

    void printInfo() {
        cerr << "Status (linhas marcadas/linhas totais) = " << countRowsHeard()
                << "/" << countRows() << endl;
    }
};

class BIn {
    size_t generation;
    matrix<uint8_t> m;
public:
    BIn(size_t g = 32) :
            generation(g), m(0, generation) {
    }

    bool addRow(std::vector<uint8_t> row) {
        assert(row.size() == generation);
        if (is_LI(m, row)) {
            ncorp::add_row(m, row);
            return true;
        } else {
            return false;
        }
    }

    std::vector<uint8_t> getRow(int index) {
        std::vector<uint8_t> result;
        matrix_row<matrix<uint8_t> > row(m, index);
        std::copy(row.begin(), row.end(), back_inserter(result));
        return result;
    }

    size_t countRows() {
        return m.size1();
    }

    matrix<uint8_t> getMatrix() {
        return m;
    }
};

struct EncodeBuffers {
    Btx btx;
    Brx brx;
    BIn bin;

    EncodeBuffers(uint32_t g) : btx(g), brx(g), bin(g){

    }
};


}  // namespace ncorp

#endif /* BUFFERS_H_ */
