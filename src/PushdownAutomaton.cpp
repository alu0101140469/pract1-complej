/**
 * @file PushdownAutomaton.cpp
 * @brief Implementa el modelo de un APv.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "PushdownAutomaton.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace cc2627 {

/**
 * @brief Construye un autómata de pila con los elementos proporcionados.
 *
 * @param states Conjunto de estados del autómata.
 * @param inputAlphabet Alfabeto de entrada (Sigma).
 * @param stackAlphabet Alfabeto de pila (Gamma).
 * @param initialState Estado inicial del autómata.
 * @param initialStackSymbol Símbolo inicial de la pila.
 * @param transitions Vector de transiciones del autómata.
 */
PushdownAutomaton::PushdownAutomaton(std::set<std::string> states,
                                     std::set<char> inputAlphabet,
                                     std::set<char> stackAlphabet,
                                     std::string initialState,
                                     char initialStackSymbol,
                                     std::vector<Transition> transitions)
    : states_(std::move(states)),
      inputAlphabet_(std::move(inputAlphabet)),
      stackAlphabet_(std::move(stackAlphabet)),
      initialState_(std::move(initialState)),
      initialStackSymbol_(initialStackSymbol),
      transitions_(std::move(transitions)) {
}

/**
 * @brief Devuelve el conjunto de estados del autómata.
 *
 * @return Conjunto de estados del autómata.
 */
const std::set<std::string>& PushdownAutomaton::getStates() const {
    return states_;
}

/**
 * @brief Devuelve el alfabeto de entrada (Sigma) del autómata.
 *
 * @return Alfabeto de entrada del autómata.
 */
const std::set<char>& PushdownAutomaton::getInputAlphabet() const {
    return inputAlphabet_;
}

/**
 * @brief Devuelve el alfabeto de pila (Gamma) del autómata.
 *
 * @return Alfabeto de pila del autómata.
 */
const std::set<char>& PushdownAutomaton::getStackAlphabet() const {
    return stackAlphabet_;
}

/**
 * @brief Devuelve el estado inicial del autómata.
 *
 * @return Estado inicial del autómata.
 */
const std::string& PushdownAutomaton::getInitialState() const {
    return initialState_;
}

/**
 * @brief Devuelve el símbolo inicial de la pila.
 *
 * @return Símbolo inicial de la pila.
 */
char PushdownAutomaton::getInitialStackSymbol() const {
    return initialStackSymbol_;
}

/**
 * @brief Devuelve el vector de transiciones del autómata.
 *
 * @return Vector de transiciones del autómata.
 */
const std::vector<Transition>& PushdownAutomaton::getTransitions() const {
    return transitions_;
}

/**
 * @brief Comprueba si un símbolo pertenece al alfabeto de entrada del autómata.
 *
 * @param symbol Símbolo a comprobar.
 * @return true si el símbolo pertenece al alfabeto de entrada, false en caso contrario.
 */
bool PushdownAutomaton::containsInputSymbol(char symbol) const {
    return inputAlphabet_.find(symbol) != inputAlphabet_.end();
}

/**
 * @brief Comprueba si un símbolo pertenece al alfabeto de pila del autómata.
 *
 * @param symbol Símbolo a comprobar.
 * @return true si el símbolo pertenece al alfabeto de pila, false en caso contrario.
 */
std::vector<std::size_t> PushdownAutomaton::getApplicableTransitionIndices(
    const Configuration& configuration) const {
    std::vector<std::size_t> result;

    if (configuration.isStackEmpty()) {
        return result;
    }

    // Obtiene el símbolo en la cima de la pila, el símbolo de entrada actual y verifica si hay transiciones aplicables.
    const char stackTop = configuration.getStack().front();
    const bool hasInput = !configuration.getRemainingInput().empty();
    const char nextInput = hasInput ? configuration.getRemainingInput().front() : '\0';

    // Itera sobre todas las transiciones y verifica cuáles son aplicables a la configuración actual.
    for (std::size_t index = 0; index < transitions_.size(); ++index) {
        const Transition& transition = transitions_[index];

        if (transition.getSourceState() != configuration.getState()
            || transition.getStackTop() != stackTop) {
            continue;
        }

        if (transition.consumesEpsilon()) {
            result.push_back(index);
            continue;
        }

        if (hasInput && transition.getInputSymbol().value() == nextInput) {
            result.push_back(index);
        }
    }

    return result;
}

/**
 * @brief Aplica una transición a una configuración dada y devuelve la nueva configuración resultante.
 *
 * @param configuration Configuración actual del autómata.
 * @param transitionIndex Índice de la transición a aplicar.
 * @return Nueva configuración resultante después de aplicar la transición.
 * @throws std::out_of_range Si el índice de transición está fuera de rango.
 * @throws std::invalid_argument Si la transición no es aplicable a la configuración indicada.
 */
Configuration PushdownAutomaton::applyTransition(const Configuration& configuration,
                                                 std::size_t transitionIndex) const {
    if (transitionIndex >= transitions_.size()) {
        throw std::out_of_range("Índice de transición fuera de rango.");
    }

    const Transition& transition = transitions_[transitionIndex];
    const std::vector<std::size_t> applicable =
        getApplicableTransitionIndices(configuration);

    if (std::find(applicable.begin(), applicable.end(), transitionIndex) == applicable.end()) {
        throw std::invalid_argument(
            "La transición no es aplicable a la configuración indicada.");
    }

    std::string remainingInput = configuration.getRemainingInput();

    if (!transition.consumesEpsilon()) {
        remainingInput.erase(0, 1);
    }

    std::string newStack = transition.getReplacement();
    newStack += configuration.getStack().substr(1);

    return Configuration(transition.getDestinationState(),
                          remainingInput,
                          newStack);
}

}