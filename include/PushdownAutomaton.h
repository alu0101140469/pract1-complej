#ifndef CC2627_PUSHDOWN_AUTOMATON_H
#define CC2627_PUSHDOWN_AUTOMATON_H

/**
 * @file PushdownAutomaton.h
 * @brief Declara el modelo de un autómata con pila por vaciado de pila.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "Configuration.h"
#include "Transition.h"

#include <set>
#include <string>
#include <vector>

namespace cc2627 {

/**
 * @class PushdownAutomaton
 * @brief Contiene la definición completa de un APv y sus operaciones básicas.
 */
class PushdownAutomaton {
public:
    /**
     * @brief Construye un APv con todos sus componentes formales.
     * @param states Conjunto de estados Q.
     * @param inputAlphabet Alfabeto de entrada Sigma.
     * @param stackAlphabet Alfabeto de pila Gamma.
     * @param initialState Estado inicial.
     * @param initialStackSymbol Símbolo inicial de la pila.
     * @param transitions Función de transición representada como lista de transiciones.
     */
    PushdownAutomaton(std::set<std::string> states,
                      std::set<char> inputAlphabet,
                      std::set<char> stackAlphabet,
                      std::string initialState,
                      char initialStackSymbol,
                      std::vector<Transition> transitions);

    /** @brief Devuelve el conjunto de estados Q. */
    const std::set<std::string>& getStates() const;

    /** @brief Devuelve el alfabeto de entrada Sigma. */
    const std::set<char>& getInputAlphabet() const;

    /** @brief Devuelve el alfabeto de pila Gamma. */
    const std::set<char>& getStackAlphabet() const;

    /** @brief Devuelve el estado inicial. */
    const std::string& getInitialState() const;

    /** @brief Devuelve el símbolo inicial de la pila. */
    char getInitialStackSymbol() const;

    /** @brief Devuelve la lista de transiciones en el orden del fichero. */
    const std::vector<Transition>& getTransitions() const;

    /** @brief Indica si un símbolo pertenece a Sigma. */
    bool containsInputSymbol(char symbol) const;

    /** @brief Obtiene las transiciones aplicables a una configuración. */
    std::vector<std::size_t> getApplicableTransitionIndices(const Configuration& configuration) const;

    /**
     * @brief Aplica una transición a una configuración válida.
     * @param configuration Configuración actual.
     * @param transitionIndex Índice de la transición que se aplicará.
     * @return Nueva configuración resultante.
     */
    Configuration applyTransition(const Configuration& configuration,
                                  std::size_t transitionIndex) const;

private:
    std::set<std::string> states_;
    std::set<char> inputAlphabet_;
    std::set<char> stackAlphabet_;
    std::string initialState_;
    char initialStackSymbol_;
    std::vector<Transition> transitions_;
};

}

#endif