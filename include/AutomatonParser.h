#ifndef CC2627_AUTOMATON_PARSER_H
#define CC2627_AUTOMATON_PARSER_H

/**
 * @file AutomatonParser.h
 * @brief Declara el lector y validador del fichero de configuración del APv.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "PushdownAutomaton.h"

#include <string>

namespace cc2627 {

/**
 * @class AutomatonParser
 * @brief Lee y valida una configuración de APv según el formato de la práctica.
 */
class AutomatonParser {
public:
    /**
     * @brief Lee un APv desde un fichero de texto.
     * @param filename Ruta del fichero de configuración.
     * @return Autómata de pila validado.
     * @throws std::runtime_error Si el fichero no puede leerse o contiene una definición inválida.
     */
    static PushdownAutomaton parseFile(const std::string& filename);
};

}

#endif