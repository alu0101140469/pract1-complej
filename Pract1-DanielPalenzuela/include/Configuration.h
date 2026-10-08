#ifndef CC2627_CONFIGURATION_H
#define CC2627_CONFIGURATION_H

/**
 * @file Configuration.h
 * @brief Declara una configuración instantánea del autómata de pila.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include <string>

namespace cc2627 {

/**
 * @class Configuration
 * @brief Representa estado, cadena restante y contenido de la pila.
 *
 * La pila se almacena con la cima en la primera posición de la cadena.
 */
class Configuration {
public:
    /**
     * @brief Construye una configuración.
     * @param state Estado actual.
     * @param remainingInput Cadena de entrada aún no consumida.
     * @param stack Contenido de la pila, con la cima en la primera posición.
     */
    Configuration(std::string state, std::string remainingInput, std::string stack);

    /** @brief Devuelve el estado actual. */
    const std::string& getState() const;

    /** @brief Devuelve la cadena restante por consumir. */
    const std::string& getRemainingInput() const;

    /** @brief Devuelve la pila, con la cima en la primera posición. */
    const std::string& getStack() const;

    /** @brief Indica si la pila está vacía. */
    bool isStackEmpty() const;

private:
    std::string state_;
    std::string remainingInput_;
    std::string stack_;
};

}

#endif