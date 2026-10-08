/**
 * @file Configuration.cpp
 * @brief Implementa la clase Configuration.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "Configuration.h"

#include <utility>

namespace cc2627 {

/**
 * @brief Construye una configuración del autómata de pila.
 *
 * @param state Estado actual del autómata.
 * @param remainingInput Cadena de entrada restante por procesar.
 * @param stack Contenido actual de la pila.
 */
Configuration::Configuration(std::string state,
                             std::string remainingInput,
                             std::string stack)
    : state_(std::move(state)),
      remainingInput_(std::move(remainingInput)),
      stack_(std::move(stack)) {
}

/**
 * @brief Devuelve el estado actual del autómata.
 *
 * @return El estado actual del autómata.
 */
const std::string& Configuration::getState() const {
    return state_;
}

/**
 * @brief Devuelve la cadena de entrada restante por procesar.
 *
 * @return La cadena de entrada restante por procesar.
 */
const std::string& Configuration::getRemainingInput() const {
    return remainingInput_;
}

/**
 * @brief Devuelve el contenido actual de la pila.
 *
 * @return El contenido actual de la pila.
 */
const std::string& Configuration::getStack() const {
    return stack_;
}

/**
 * @brief Comprueba si la pila está vacía.
 *
 * @return true si la pila está vacía, false en caso contrario.
 */
bool Configuration::isStackEmpty() const {
    return stack_.empty();
}

}