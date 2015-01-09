/*
 * Generation.cc
 *
 *  Created on: Mar 18, 2013
 *      Author: ivocalado
 */

#include "Generation.h"
#include "Binary8MatrixHelpers.h"
#include "HashMatrices.h"
#include <kodo/rlnc/full_vector_codes.hpp>
#include <stdio.h>
#include <sstream>
#include "packets_m.h"
#include "helper-functions.h"
#include <functional>
namespace ncorp {

template<class T> void print(matrix<T> &a) {
    for (size_t i = 0; i < a.size1(); i++) {
        cerr << "(";
        for (size_t j = 0; j < a.size2(); j++) {
            cerr << static_cast<int>(a(i, j))
                    << ((j == (a.size2() - 1)) ? "" : " ");
        }
        cerr << ")" << endl;
    }
}

template<class T> void print(std::vector<T> &v) {
    stringstream st;
    for (auto i = 0U; i < v.size(); i++) {
        st << static_cast<int>(v[i]) << ((i == (v.size() - 1)) ? "" : " ");
    }

    debugprintf(stderr, LOG_LEVEL_4, "Linha recebida: %s\n", st.str().c_str());
}

void print(Brx &brx) {
    debugprintf(stderr, LOG_LEVEL_4, "Status atual do BRX\n");
    int rows = brx.countRows();
    debugprintf(stderr, LOG_LEVEL_4, "rows = %d\n", rows);

    for (int j = 0; j < rows; j++) {
        stringstream st;
        std::vector<uint8_t> a = brx.getRow(j);
        int counter = brx.rowCounter(j);
        bool heard = brx.isRowHeard(j);
        st << "(";
        for (size_t i = 0; i < a.size(); i++) {

            st << static_cast<int>(a[i]) << ((i == (a.size() - 1)) ? "" : " ");
        }
        st << ") " << counter << (heard ? " *****\n" : " NOT heard\n");
        debugprintf(stderr, LOG_LEVEL_4, "%s", st.str().c_str());
    }

}

void print(Btx &btx) {
    debugprintf(stderr, LOG_LEVEL_4, "Status atual do BTX\n");
    int rows = btx.countRows();
    debugprintf(stderr, LOG_LEVEL_4, "rows = %d\n", rows);

    for (int j = 0; j < rows; j++) {
        stringstream st;
        std::vector<uint8_t> a = btx.getRow(j);
        bool heard = btx.isRowHeard(j);
        st << "(";
        for (size_t i = 0; i < a.size(); i++) {

            st << static_cast<int>(a[i]) << ((i == (a.size() - 1)) ? "" : " ");
        }
        st << ") " << (heard ? " *****\n" : " NOT heard\n");
        debugprintf(stderr, LOG_LEVEL_4, "%s", st.str().c_str());
    }

}

uint8_t rand_non_zero(uint8_t limit) {
    uint8_t result = rand() % limit;
    while (!result)
        result = rand() % limit;
    return result;
}

#include <iostream>
using namespace std;

Generation::Generation(uint32_t nId, uint16_t ident, /*Role rl, */uint32_t sb,
        uint32_t symbol_sz, std::vector<uint8_t> rawBlock, size_t pl) :
        nodeId(nId), id(ident), generation_size(sb), packet_size(symbol_sz), encoder_factory(
                generation_size, packet_size), decoder_factory(generation_size,
                packet_size), info_decoder_factory(generation_size,
                packet_size), ebuffers(generation_size), role(SENDER), timeout(0), payloadSize(pl)

{
    encoder = encoder_factory.build();
    //considera-se que rawBlock.size() == encoder->block_size()
    encoder->set_symbols(sak::storage(rawBlock));
    if (kodo::is_systematic_encoder(encoder)) //
        kodo::set_systematic_off(encoder); //
    timeout = new cMessage("generation-timeout", GENERATION_TIMEOUT);
    t0 = t_eack = -1.0;
    ackedGenerations = 0;
}

Generation::Generation(uint32_t nId, uint16_t ident, Role rl, uint32_t sb,
        uint32_t symbol_sz, size_t pl) :
        nodeId(nId), id(ident), generation_size(sb), packet_size(symbol_sz), encoder_factory(
                generation_size, packet_size), decoder_factory(generation_size,
                packet_size), info_decoder_factory(generation_size,
                packet_size), ebuffers(generation_size), role(rl), timeout(0), payloadSize(pl) {
    decoder = decoder_factory.build();
    timeout = new cMessage("generation-timeout", GENERATION_TIMEOUT);
    markNextPktAsInovative = false;

    t0 = t_eack = -1.0;
}

size_t Generation::getPayloadSize() const {
    return payloadSize;
}

std::shared_ptr<std::vector<uint8_t> > Generation::getDecodedBlock() {
    //Retorna o bloco decodificado
    return data_out;
}

void Generation::pushAckCoding(std::vector<uint8_t> payload, uint32_t seed) {
    debugprintf(stderr, LOG_LEVEL_4, "Generation::pushAckCoding Begin\n");
    print(payload);
    size_t m = 4; //Tamanho da matriz
    HashMatrices hash(m, generation_size, seed); //gera m matrizes 32 x 32 com base no seed especificado
    auto z = transpose(convert_to_matrix(payload));

    //Actions for Brx
    for (size_t i = 0; i < ebuffers.brx.countRows(); i++) {
        auto w = convert_to_matrix(ebuffers.brx.getRow(i));
        bool mark = true;
        for (size_t j = 0; j < m; j++) {
            auto hash_n = hash.getMatrix(j);
            auto tmp = multiply(multiply(w, hash_n), z);
            if (!is_zero(tmp)) {
                mark = false;
                break;
            }
        }

        if (mark) {
            ebuffers.brx.markRowAsHeard(i);
        }
    }

    print(ebuffers.brx);

    //Actions for Btx
    for (size_t i = 0; i < ebuffers.btx.countRows(); i++) {
        auto w = convert_to_matrix(ebuffers.btx.getRow(i));
        bool mark = true;
        for (size_t j = 0; j < m; j++) {
            auto hash_n = hash.getMatrix(j);
            auto tmp = multiply(multiply(w, hash_n), z);
            if (!is_zero(tmp)) {
                mark = false;
                break;
            }
        }

        if (mark) {
            ebuffers.btx.markRowAsHeard(i);
        }

    }

    print(ebuffers.btx);
    debugprintf(stderr, LOG_LEVEL_4, "Generation::pushAckCoding End\n");
} //Manipula o vetor de codificação

bool Generation::isComplete() {
    auto result = decoder->is_complete();
    return result;
} //Retorna verdadeiro se a geração estiver completa. Este método é utilizado pelo receiver para informar que já foram recebidos todos os pacotes da geração

//Calcula o diferencial backlog. Se o parâmetro passado for true calcula do enconder. Caso contrário do decoder
double Generation::calculateDifferentialBacklog() {

    switch (role) {
    case SENDER: {
        auto result = ebuffers.brx.getHmatrix(); //Recupera as linhas ouvidas de brx
        concat_vertically(result, ebuffers.btx.getHmatrix()); //recupera e concatena as linhas ouvidas de btx ao final de brx
        auto dimensionH = calculate_dimension(result);

        auto dimensionIn = encoder->symbols_available(); //Recupera a dimensao do buffer de envio
        assert(dimensionH <= dimensionIn); //Obrigatoriamente a dimensao de saida tem de ser maior ou igual a de entrada
        return dimensionIn - dimensionH;
    }
        break;
    case RELAY: {
        auto result = ebuffers.brx.getHmatrix(); //Recupera as linhas ouvidas de brx
        concat_vertically(result, ebuffers.btx.getHmatrix()); //recupera e concatena as linhas ouvidas de btx ao final de brx
        int dimensionH = calculate_dimension(result);
        int bIn = rank(ebuffers.bin.getMatrix());
        assert(dimensionH <= bIn);
        return bIn - dimensionH;
    }
        break;
    case RECEIVER:
    default:
        return 0;
    }

}

bool Generation::isTransmitting() {
    return calculateDifferentialBacklog() > 0;
}

Generation::~Generation() {
    if (timeout)
        delete timeout;
}

int i = 0;
void Generation::pushEncodedData(
        std::shared_ptr<std::vector<uint8_t> > payload) {
    debugprintf(stderr, LOG_LEVEL_4, "Generation::pushEncodedData Begin\n");

    auto info_decoder = info_decoder_factory.build();
    info_decoder->decode(&(*payload.get())[0]);
    if (info_decoder->cached_symbol_coded()) {
        const uint8_t* c = info_decoder->cached_symbol_coefficients();
        std::vector<fifi::binary8::value_type> coefficients;
        for (uint32_t i = 0; i < info_decoder->symbols(); ++i) {
            coefficients.push_back(fifi::get_value<fifi::binary8>(c, i));
        }

        print(coefficients);

        if (role == RECEIVER) {
            debugprintf(stderr, LOG_LEVEL_4, "[Time = %f] Packet received!!\n",
                    simTime().dbl());
        }

        if (ebuffers.bin.addRow(coefficients)) {
            markNextPktAsInovative = true;
            if (role == RECEIVER)
                fprintf(stderr, "%d %f\n",
                        ++i, simTime().dbl());
            decoder->decode(&(*payload.get())[0]);
        }

        //apos a recuperacao dos coeficientes, deve-se adicionado ao brx
        ebuffers.brx.addRow(coefficients);
        print(ebuffers.brx);
    }

    if (isComplete() && role == RECEIVER) {
        debugprintf(stderr, LOG_LEVEL_4,
                "Geracao completa. Entregando dados à aplicação\n");
        data_out.reset(new std::vector<uint8_t>(decoder->block_size()));
        decoder->copy_symbols(sak::storage(*data_out.get()));
        data_out->erase(data_out->begin() + payloadSize, data_out->end());
    }

    debugprintf(stderr, LOG_LEVEL_4, "Generation::pushEncodedData End\n");
}

std::tuple<std::vector<uint8_t>, std::shared_ptr<std::vector<uint8_t> >, bool> Generation::generateEncodedPacket() {
    std::shared_ptr<std::vector<uint8_t> > payload;
    std::vector<uint8_t> coefficients;
    bool isInovative;
    if (role == SENDER) {
        payload.reset(new std::vector<uint8_t>(encoder->payload_size()));
        encoder->encode(&(*payload.get())[0]); //Equivalente a &payload[0] de um vector<uint8_t>
        auto info_decoder = info_decoder_factory.build();
        info_decoder->decode(&(*payload.get())[0]);
        if (info_decoder->cached_symbol_coded()) {
            const uint8_t* c = info_decoder->cached_symbol_coefficients();
            for (uint32_t i = 0; i < info_decoder->symbols(); ++i) {
                coefficients.push_back(fifi::get_value<fifi::binary8>(c, i));
            }

            //apos a recuperacao dos coeficientes, deve-se adicionado ao btx
            ebuffers.btx.addRow(coefficients);
//            print(coefficients, nodeId);
        }
        isInovative = true;

    } else {

        payload.reset(new std::vector<uint8_t>(decoder->payload_size()));
        decoder->recode(&(*payload.get())[0]);
        auto info_decoder = info_decoder_factory.build();
        info_decoder->decode(&(*payload.get())[0]);
        if (info_decoder->cached_symbol_coded()) {
            const uint8_t* c = info_decoder->cached_symbol_coefficients();
            for (uint32_t i = 0; i < info_decoder->symbols(); ++i) {
                coefficients.push_back(fifi::get_value<fifi::binary8>(c, i));
            }

            //apos a recuperacao dos coeficientes, deve-se adicionado ao btx
            ebuffers.btx.addRow(coefficients);

//            print(coefficients, nodeId);
        }

        isInovative = markNextPktAsInovative;
        markNextPktAsInovative = false;
    }
    return std::make_tuple(coefficients, payload, isInovative);
}

std::vector<uint8_t> Generation::retrieveCurrentAckVector() {
    size_t m = 4; //Numero de matrizes Hash
    size_t n = generation_size;
    matrix<uint8_t> phi(zero_matrix<uint8_t>(0, n)); //gera uma matrix inicial 0 x 32
    int seed = nodeId;
    HashMatrices hash(m, n, seed); //gera m matrizes 32 x 32 com base no seed especificado

    size_t selectedRows = 0;

    do {
        int lowestIndex = ebuffers.brx.getLowestCountedRow();
        auto u = ebuffers.brx.getRow(lowestIndex); //recupera a linha com menor contagem
        if (is_LI(phi, u)) { //verifica se eh LI
            add_row(phi, u);    //adiciona a linha
        }
        to_reduced_row_echelon_form(phi); //Converte para a forma normal escalonada
        ebuffers.brx.incrementRowCounter(lowestIndex); //Incrementa o contador
        selectedRows++;
    } while (!((phi.size1() == static_cast<unsigned int>((n / m - 1)))
            || selectedRows == ebuffers.brx.countRows()));

    size_t columnsToReduce = phi.size1() * m;

    matrix<uint8_t> phi_(zero_matrix<uint8_t>(0, n)); //Inicia uma segunda matrix 0 x 32

    for (size_t i = 0; i < m; i++) {
        matrix<uint8_t> m = multiply(phi, hash.getMatrix(i));
        concat_vertically(phi_, m); //Concatena verticalmente a phi_
    }

    to_reduced_row_echelon_form(phi_);

    std::vector<uint8_t> result(generation_size);
    for (size_t i = columnsToReduce; i < generation_size; i++)
        result[i] = rand_non_zero(255);

    ncorp::binary8_arithmetic_traits ar;
    for (size_t i = 0; i < columnsToReduce; i++) {
        uint8_t r = ar.zero();
        for (size_t j = columnsToReduce; j < n; j++) {
            r = ar.add(r, ar.mul(result[j], phi_(i, j)));
        }

        result[i] = ar.negate(r);
    }
    return result;
}

uint16_t Generation::getId() const {
    return id;
}

cMessage* Generation::getTimeoutMsg() {
    return timeout;
}

void Generation::printInfo() {

}

//Marca o inicio da transmissão, ajustando o t0
void Generation::startTransmit() {
    if (t0 < SIMTIME_ZERO)
        t0 = simTime();
}

//Marca o fim da transmissão, ajustando o t_eack
void Generation::stopTransmit() {
    if (t0 > SIMTIME_ZERO && t_eack < SIMTIME_ZERO)
        t_eack = simTime();
}

//Calcula DT, tempo de entrega
simtime_t Generation::calculateDT() {
    return t_eack < SIMTIME_ZERO ? MAXTIME: t_eack - t0;
}

void Generation::notifyGenerationReception() {
    if (t0 > SIMTIME_ZERO)
        ackedGenerations++;
}

double Generation::calculateActualSendingRate() {
    return static_cast<double>(ackedGenerations) / calculateDT();
}
} /* namespace ncorp */
