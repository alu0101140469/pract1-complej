/**
 * @file TraceWriter.cpp
 * @brief Implementa la generación de la traza del APv.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "TraceWriter.h"

namespace cc2627 {
namespace {

/**
 * @brief Devuelve una representación de la cadena de entrada, mostrando epsilon si está vacía.
 *
 * @param input La cadena de entrada.
 * @return Una representación de la cadena de entrada.
 */
std::string displayInput(const std::string& input) {
    return input.empty() ? "epsilon" : input;
}

/**
 * @brief Devuelve una representación de la pila, mostrando epsilon si está vacía.
 *
 * @param stack La cadena que representa la pila.
 * @return Una representación de la pila.
 */
std::string displayStack(const std::string& stack) {
    return stack.empty()
        ? "epsilon"
        : stack + " (cima -> fondo)";
}

/**
 * @brief Escribe la configuración actual del autómata en el flujo de salida.
 *
 * @param automaton El autómata de pila.
 * @param configuration La configuración actual del autómata.
 * @param output Flujo de salida donde se escribirá la configuración.
 */
void writeConfiguration(
    const PushdownAutomaton& automaton,
    const Configuration& configuration,
    std::ostream& output) {

    output << "  Estado: "
           << configuration.getState()
           << '\n';

    output << "  Cadena restante: "
           << displayInput(
                  configuration.getRemainingInput())
           << '\n';

    output << "  Pila: "
           << displayStack(
                  configuration.getStack())
           << '\n';

    const std::vector<std::size_t> applicable =
        automaton.getApplicableTransitionIndices(
            configuration);

    output << "  Transiciones posibles: ";

    if (applicable.empty()) {
        output << "ninguna\n";
        return;
    }

    for (std::size_t index = 0;
         index < applicable.size();
         ++index) {

        if (index != 0) {
            output << "; ";
        }

        output << 'T'
               << (applicable[index] + 1)
               << " ["
               << automaton.getTransitions()
                      [applicable[index]]
                      .toString()
               << ']';
    }

    output << '\n';
}

}

/**
 * @brief Escribe la traza de la ejecución del autómata de pila para una palabra dada.
 *
 * @param automaton El autómata de pila.
 * @param word La palabra a reconocer.
 * @param result Resultado del reconocimiento de la palabra.
 * @param output Flujo de salida donde se escribirá la traza.
 */
void TraceWriter::write(
    const PushdownAutomaton& automaton,
    const std::string& word,
    const RecognitionResult& result,
    std::ostream& output) {

    output
        << "============================================================\n";

    output
        << "Traza de la palabra: "
        << displayInput(word)
        << '\n';

    output
        << "============================================================\n";

    Configuration configuration(
        automaton.getInitialState(),
        word,
        std::string(
            1,
            automaton.getInitialStackSymbol()));

    output << "Configuración inicial:\n";

    writeConfiguration(
        automaton,
        configuration,
        output);

    if (!result.inputValid) {
        output
            << "\nNo se puede ejecutar la traza: "
            << result.message
            << '\n';

        output << "\n";
        return;
    }

    if (!result.accepted) {
        output
            << "\nNo existe un camino aceptante "
               "para esta palabra.\n";

        output
            << "Resultado: NO PERTENECE\n\n";

        return;
    }

    const std::vector<Transition>& transitions =
        automaton.getTransitions();

    for (std::size_t step = 0;
         step < result.acceptingTransitionIndices.size();
         ++step) {

        const std::size_t transitionIndex =
            result.acceptingTransitionIndices[step];

        configuration =
            automaton.applyTransition(
                configuration,
                transitionIndex);

        output << '\n';

        output << "Paso "
               << (step + 1)
               << ": T"
               << (transitionIndex + 1)
               << " -> "
               << transitions[transitionIndex].toString()
               << '\n';

        writeConfiguration(
            automaton,
            configuration,
            output);
    }

    output
        << '\n'
        << "Resultado: PERTENECE "
           "(pila vacía y entrada consumida)\n\n";
}

}