#ifndef CC2627_TRACE_WRITER_H
#define CC2627_TRACE_WRITER_H

/**
 * @file TraceWriter.h
 * @brief Declara el componente encargado de generar la traza del APv.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "PushdownAutomaton.h"
#include "PushdownRecognizer.h"

#include <ostream>
#include <string>

namespace cc2627 {

/**
 * @class TraceWriter
 * @brief Escribe una traza legible después de cada transición del camino aceptante.
 */
class TraceWriter {
public:
    /**
     * @brief Escribe la traza asociada a una palabra.
     * @param automaton APv utilizado.
     * @param word Palabra analizada.
     * @param result Resultado del reconocimiento.
     * @param output Flujo destino de la traza.
     */
    static void write(const PushdownAutomaton& automaton,
                      const std::string& word,
                      const RecognitionResult& result,
                      std::ostream& output);
};

}

#endif