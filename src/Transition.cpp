/**
 * @file Transition.cpp
 * @brief Implementa la clase Transition.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "Transition.h"

#include <sstream>
#include <utility>

namespace cc2627 {

/**
 * @brief Construye una transición del autómata de pila.
 *
 * @param sourceState Estado de origen de la transición.
 * @param inputSymbol Símbolo de entrada que activa la transición (opcional).
 * @param stackTop Símbolo en la cima de la pila que activa la transición.
 * @param destinationState Estado de destino de la transición.
 * @param replacement Cadena que reemplaza al símbolo en la cima de la pila.
 */
Transition::Transition(std::string sourceState,
                       std::optional<char> inputSymbol,
                       char stackTop,
                       std::string destinationState,
                       std::string replacement)
    : sourceState_(std::move(sourceState)),
      inputSymbol_(inputSymbol),
      stackTop_(stackTop),
      destinationState_(std::move(destinationState)),
      replacement_(std::move(replacement)) {
}

/**
 * @brief Devuelve el estado de origen de la transición.
 *
 * @return Estado de origen de la transición.
 */
const std::string& Transition::getSourceState() const {
    return sourceState_;
}

/**
 * @brief Devuelve el símbolo de entrada que activa la transición (opcional).
 *
 * @return Símbolo de entrada que activa la transición, o std::nullopt si es una transición epsilon.
 */
std::optional<char> Transition::getInputSymbol() const {
    return inputSymbol_;
}

/**
 * @brief Devuelve el símbolo en la cima de la pila que activa la transición.
 *
 * @return Símbolo en la cima de la pila que activa la transición.
 */
char Transition::getStackTop() const {
    return stackTop_;
}

/**
 * @brief Devuelve el estado de destino de la transición.
 *
 * @return Estado de destino de la transición.
 */
const std::string& Transition::getDestinationState() const {
    return destinationState_;
}

/**
 * @brief Devuelve la cadena que reemplaza al símbolo en la cima de la pila.
 *
 * @return Cadena que reemplaza al símbolo en la cima de la pila.
 */
const std::string& Transition::getReplacement() const {
    return replacement_;
}

/**
 * @brief Comprueba si la transición es una transición epsilon (sin consumir símbolo de entrada).
 *
 * @return true si la transición es epsilon, false en caso contrario.
 */
bool Transition::consumesEpsilon() const {
    return !inputSymbol_.has_value();
}

/**
 * @brief Devuelve una representación en cadena de la transición.
 *
 * @return Representación en cadena de la transición.
 */
std::string Transition::toString() const {
    std::ostringstream out;
    out << sourceState_ << ' ';
    out << (inputSymbol_.has_value() ? std::string(1, *inputSymbol_) : ".");
    out << ' ' << stackTop_ << ' ' << destinationState_ << ' ';
    out << (replacement_.empty() ? "." : replacement_);
    return out.str();
}

}