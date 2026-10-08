#ifndef CC2627_TRANSITION_H
#define CC2627_TRANSITION_H

/**
 * @file Transition.h
 * @brief Declara la transición de un autómata de pila.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include <optional>
#include <string>

namespace cc2627 {

/**
 * @class Transition
 * @brief Representa una transición de un autómata de pila.
 *
 * El punto (.) utilizado en el fichero de configuración se representa
 * internamente como ausencia de símbolo de entrada o de símbolos de reposición.
 */
class Transition {
public:
    /**
     * @brief Construye una transición.
     * @param sourceState Estado origen.
     * @param inputSymbol Símbolo de entrada o std::nullopt para epsilon.
     * @param stackTop Símbolo que debe estar en la cima de la pila.
     * @param destinationState Estado destino.
     * @param replacement Cadena que sustituye al símbolo de la cima; vacía significa epsilon.
     */
    Transition(std::string sourceState,
               std::optional<char> inputSymbol,
               char stackTop,
               std::string destinationState,
               std::string replacement);

    /** @brief Devuelve el estado origen. */
    const std::string& getSourceState() const;

    /** @brief Devuelve el símbolo de entrada o std::nullopt para epsilon. */
    std::optional<char> getInputSymbol() const;

    /** @brief Devuelve el símbolo esperado en la cima de la pila. */
    char getStackTop() const;

    /** @brief Devuelve el estado destino. */
    const std::string& getDestinationState() const;

    /** @brief Devuelve la cadena de reposición de la pila. */
    const std::string& getReplacement() const;

    /** @brief Indica si la transición es epsilon en la entrada. */
    bool consumesEpsilon() const;

    /** @brief Devuelve una representación legible de la transición. */
    std::string toString() const;

private:
    std::string sourceState_;
    std::optional<char> inputSymbol_;
    char stackTop_;
    std::string destinationState_;
    std::string replacement_;
};

}

#endif