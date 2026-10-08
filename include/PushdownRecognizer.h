#ifndef CC2627_PUSHDOWN_RECOGNIZER_H
#define CC2627_PUSHDOWN_RECOGNIZER_H

/**
 * @file PushdownRecognizer.h
 * @brief Declara el reconocedor de palabras para un APv.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "PushdownAutomaton.h"

#include <cstddef>
#include <string>
#include <vector>

namespace cc2627 {

/**
 * @struct RecognitionResult
 * @brief Contiene el resultado de reconocer una palabra y si existe, un camino aceptante.
 */
struct RecognitionResult {
    bool accepted = false;
    bool inputValid = true;
    std::string message;
    std::vector<std::size_t> acceptingTransitionIndices;
};

/**
 * @class PushdownRecognizer
 * @brief Reconoce palabras de un APv mediante una construcción equivalente de gramática libre de contexto.
 *
 * El algoritmo evita depender de una única elección entre transiciones no deterministas y
 * permite decidir también casos en los que existen ciclos epsilon que aumentan la pila.
 */
class PushdownRecognizer {
public:
    /**
     * @brief Construye un reconocedor asociado al APv indicado.
     * @param automaton Autómata que se utilizará para reconocer las palabras.
     */
    explicit PushdownRecognizer(const PushdownAutomaton& automaton);

    /**
     * @brief Comprueba si una palabra pertenece al lenguaje reconocido por el APv.
     * @param word Palabra de entrada, representada como una cadena de caracteres.
     * @return Resultado del reconocimiento y camino aceptante cuando existe.
     */
    RecognitionResult recognize(const std::string& word) const;

private:
    const PushdownAutomaton& automaton_;
};

}

#endif