/**
 * @file PushdownRecognizer.cpp
 * @brief Implementa el reconocimiento completo de palabras de un APv.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "PushdownRecognizer.h"

#include <cstdint>
#include <functional>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cc2627 {
namespace {

/**
 * @brief Representa una producción de la gramática generada a partir del APv.
 */
struct Production {
    int leftVariable = -1;
    std::optional<char> inputSymbol;
    std::size_t transitionIndex = 0;
    std::vector<int> rightVariables;
};

/**
 * @brief Representa un testigo de una producción aplicada en el reconocimiento.
 */
struct Witness {
    std::size_t productionIndex = 0;
    std::vector<std::pair<int, int>> childSpans;
};

class GrammarEngine {
public:
    /**
     * @brief Construye un motor de gramática a partir de un APv y una palabra.
     *
     * @param automaton El autómata de pila.
     * @param word La palabra a reconocer.
     */
    GrammarEngine(const PushdownAutomaton& automaton,
                  const std::string& word)
        : automaton_(automaton),
          word_(word),
          states_(automaton.getStates().begin(),
                  automaton.getStates().end()),
          stackSymbols_(automaton.getStackAlphabet().begin(),
                        automaton.getStackAlphabet().end()),
          stateIndex_(),
          stackIndex_(),
          variableCount_(
              states_.size()
              * stackSymbols_.size()
              * states_.size()),
          productions_(),
          chart_(),
          witnesses_() {

        for (std::size_t index = 0;
             index < states_.size();
             ++index) {
            stateIndex_.emplace(
                states_[index],
                static_cast<int>(index));
        }

        for (std::size_t index = 0;
             index < stackSymbols_.size();
             ++index) {
            stackIndex_.emplace(
                stackSymbols_[index],
                static_cast<int>(index));
        }

        buildProductions();
    }

    /**
     * @brief Realiza el reconocimiento de la palabra y devuelve si pertenece al lenguaje del APv.
     *
     * @param path Vector donde se almacenará el camino de transiciones si la palabra es aceptada.
     * @return true si la palabra pertenece al lenguaje del APv, false en caso contrario.
     */
    bool recognize(std::vector<std::size_t>& path) {
        saturateChart();

        const int initialState =
            stateIndex_.at(automaton_.getInitialState());

        const int initialStack =
            stackIndex_.at(automaton_.getInitialStackSymbol());

        const int length =
            static_cast<int>(word_.size());

        for (std::size_t finalState = 0;
             finalState < states_.size();
             ++finalState) {

            const int rootVariable = variableId(
                initialState,
                initialStack,
                static_cast<int>(finalState));

            const std::uint64_t key =
                makeKey(rootVariable, 0, length);

            if (chart_.find(key) == chart_.end()) {
                continue;
            }

            reconstruct(
                rootVariable,
                0,
                length,
                path);

            return true;
        }

        return false;
    }

private:
    /**
     * @brief Genera una clave única para una variable y un rango de la palabra.
     *
     * @param variable Identificador de la variable.
     * @param begin Índice de inicio del rango (inclusive).
     * @param end Índice de fin del rango (exclusive).
     * @return Una clave única que representa la variable y el rango.
     */
    std::uint64_t makeKey(
        int variable,
        int begin,
        int end) const {

        const std::uint64_t n =
            static_cast<std::uint64_t>(
                word_.size() + 1);

        return (
            (
                static_cast<std::uint64_t>(variable)
                * n
                + static_cast<std::uint64_t>(begin)
            )
            * n
            + static_cast<std::uint64_t>(end)
        );
    }

    /**
     * @brief Calcula un identificador único para una variable basada en el estado de origen, el símbolo de pila y el estado de destino.
     *
     * @param fromState Índice del estado de origen.
     * @param stackSymbol Índice del símbolo de pila.
     * @param toState Índice del estado de destino.
     * @return Un identificador único para la variable.
     */
    int variableId(
        int fromState,
        int stackSymbol,
        int toState) const {

        const int stateCount =
            static_cast<int>(states_.size());

        const int stackCount =
            static_cast<int>(stackSymbols_.size());

        return (
            fromState * stackCount
            + stackSymbol
        ) * stateCount + toState;
    }

    /**
     * @brief Construye las producciones de la gramática a partir de las transiciones del APv.
     */
    void buildProductions() {
        const std::vector<Transition>& transitions =
            automaton_.getTransitions();

        for (std::size_t transitionIndex = 0;
             transitionIndex < transitions.size();
             ++transitionIndex) {

            const Transition& transition =
                transitions[transitionIndex];

            const int sourceState =
                stateIndex_.at(
                    transition.getSourceState());

            const int topSymbol =
                stackIndex_.at(
                    transition.getStackTop());

            const std::string& replacement =
                transition.getReplacement();

            if (replacement.empty()) {
                const int leftVariable =
                    variableId(
                        sourceState,
                        topSymbol,
                        stateIndex_.at(
                            transition.getDestinationState()));

                addProduction(
                    leftVariable,
                    transition.getInputSymbol(),
                    transitionIndex,
                    {});

                continue;
            }

            std::vector<int> intermediateStates(
                replacement.size() - 1,
                0);

            std::function<void(std::size_t)> generate =
                [&](std::size_t position) {

                if (position == intermediateStates.size()) {
                    for (std::size_t finalState = 0;
                         finalState < states_.size();
                         ++finalState) {

                        std::vector<int> rightVariables;
                        rightVariables.reserve(
                            replacement.size());

                        int fromState =
                            stateIndex_.at(
                                transition.getDestinationState());

                        for (std::size_t replacementIndex = 0;
                             replacementIndex < replacement.size();
                             ++replacementIndex) {

                            int toState =
                                static_cast<int>(finalState);

                            if (replacementIndex
                                < intermediateStates.size()) {
                                toState =
                                    intermediateStates[
                                        replacementIndex];
                            }

                            rightVariables.push_back(
                                variableId(
                                    fromState,
                                    stackIndex_.at(
                                        replacement[
                                            replacementIndex]),
                                    toState));

                            fromState = toState;
                        }

                        const int leftVariable =
                            variableId(
                                sourceState,
                                topSymbol,
                                static_cast<int>(finalState));

                        addProduction(
                            leftVariable,
                            transition.getInputSymbol(),
                            transitionIndex,
                            std::move(rightVariables));
                    }

                    return;
                }

                for (std::size_t state = 0;
                     state < states_.size();
                     ++state) {

                    intermediateStates[position] =
                        static_cast<int>(state);

                    generate(position + 1);
                }
            };

            generate(0);
        }
    }

    /**
     * @brief Agrega una producción a la lista de producciones.
     *
     * @param leftVariable Identificador de la variable izquierda.
     * @param inputSymbol Símbolo de entrada opcional.
     * @param transitionIndex Índice de la transición correspondiente.
     * @param rightVariables Lista de variables derechas.
     */
    void addProduction(
        int leftVariable,
        std::optional<char> inputSymbol,
        std::size_t transitionIndex,
        std::vector<int> rightVariables) {

        productions_.push_back(
            Production{
                leftVariable,
                inputSymbol,
                transitionIndex,
                std::move(rightVariables)
            });
    }

    /**
     * @brief Comprueba si una variable y un rango de la palabra están presentes en el chart.
     *
     * @param variable Identificador de la variable.
     * @param begin Índice de inicio del rango (inclusive).
     * @param end Índice de fin del rango (exclusive).
     * @return true si la variable y el rango están presentes en el chart, false en caso contrario.
     */
    bool contains(
        int variable,
        int begin,
        int end) const {

        return chart_.find(
            makeKey(variable, begin, end))
            != chart_.end();
    }

    /**
     * @brief Intenta hacer coincidir una secuencia de variables derechas con un rango de la palabra.
     *
     * @param variables Secuencia de variables derechas.
     * @param begin Índice de inicio del rango (inclusive).
     * @param end Índice de fin del rango (exclusive).
     * @param childSpans Vector donde se almacenarán los rangos de las variables hijas si la coincidencia es exitosa.
     * @return true si la secuencia de variables coincide con el rango, false en caso contrario.
     */
    bool matchRightVariables(
        const std::vector<int>& variables,
        int begin,
        int end,
        std::vector<std::pair<int, int>>& childSpans) const {

        if (variables.empty()) {
            return begin == end;
        }

        std::vector<std::vector<int>> predecessors(
            variables.size(),
            std::vector<int>(
                word_.size() + 1,
                -1));

        std::vector<int> reachable{begin};

        for (std::size_t step = 0;
             step < variables.size();
             ++step) {

            std::vector<int> next;

            for (const int current : reachable) {
                if (current > end) {
                    continue;
                }

                for (int nextPosition = current;
                     nextPosition <= end;
                     ++nextPosition) {

                    if (!contains(
                            variables[step],
                            current,
                            nextPosition)) {
                        continue;
                    }

                    if (predecessors[step][nextPosition] == -1) {
                        predecessors[step][nextPosition] =
                            current;

                        next.push_back(nextPosition);
                    }
                }
            }

            reachable = std::move(next);

            if (reachable.empty()) {
                return false;
            }
        }

        if (predecessors.back()[end] == -1) {
            return false;
        }

        childSpans.assign(
            variables.size(),
            {});

        int current = end;

        for (std::size_t step = variables.size();
             step > 0;
             --step) {

            const int beginOfStep =
                predecessors[step - 1][current];

            childSpans[step - 1] =
                {beginOfStep, current};

            current = beginOfStep;
        }

        return current == begin;
    }

    /**
     * @brief Realiza la saturación del chart aplicando todas las producciones posibles.
     */
    void saturateChart() {
        bool changed = true;
        const int length =
            static_cast<int>(word_.size());

        while (changed) {
            changed = false;

            for (std::size_t productionIndex = 0;
                 productionIndex < productions_.size();
                 ++productionIndex) {

                const Production& production =
                    productions_[productionIndex];

                const int firstConsumed =
                    production.inputSymbol.has_value()
                        ? 1
                        : 0;

                for (int begin = 0;
                     begin <= length;
                     ++begin) {

                    const int afterInput =
                        begin + firstConsumed;

                    if (afterInput > length) {
                        continue;
                    }

                    if (production.inputSymbol.has_value()
                        && word_[
                               static_cast<std::size_t>(
                                   begin)]
                           != *production.inputSymbol) {
                        continue;
                    }

                    if (production.rightVariables.empty()) {
                        const std::uint64_t key =
                            makeKey(
                                production.leftVariable,
                                begin,
                                afterInput);

                        if (chart_.insert(key).second) {
                            witnesses_.emplace(
                                key,
                                Witness{
                                    productionIndex,
                                    {}
                                });

                            changed = true;
                        }

                        continue;
                    }

                    for (int end = afterInput;
                         end <= length;
                         ++end) {

                        std::vector<std::pair<int, int>>
                            childSpans;

                        if (!matchRightVariables(
                                production.rightVariables,
                                afterInput,
                                end,
                                childSpans)) {
                            continue;
                        }

                        const std::uint64_t key =
                            makeKey(
                                production.leftVariable,
                                begin,
                                end);

                        if (chart_.insert(key).second) {
                            witnesses_.emplace(
                                key,
                                Witness{
                                    productionIndex,
                                    std::move(childSpans)
                                });

                            changed = true;
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Reconstruye el camino de transiciones que llevó al reconocimiento de la palabra.
     *
     * @param variable Identificador de la variable actual.
     * @param begin Índice de inicio del rango (inclusive).
     * @param end Índice de fin del rango (exclusive).
     * @param path Vector donde se almacenará el camino de transiciones.
     */
    void reconstruct(
        int variable,
        int begin,
        int end,
        std::vector<std::size_t>& path) const {

        const std::uint64_t key =
            makeKey(variable, begin, end);

        const Witness& witness =
            witnesses_.at(key);

        const Production& production =
            productions_[witness.productionIndex];

        path.push_back(
            production.transitionIndex);

        for (std::size_t index = 0;
             index < production.rightVariables.size();
             ++index) {

            const auto [childBegin, childEnd] =
                witness.childSpans[index];

            reconstruct(
                production.rightVariables[index],
                childBegin,
                childEnd,
                path);
        }
    }

    const PushdownAutomaton& automaton_;
    const std::string& word_;

    std::vector<std::string> states_;
    std::vector<char> stackSymbols_;

    std::map<std::string, int> stateIndex_;
    std::map<char, int> stackIndex_;

    std::size_t variableCount_;

    std::vector<Production> productions_;

    std::unordered_set<std::uint64_t> chart_;

    std::unordered_map<
        std::uint64_t,
        Witness> witnesses_;
};

}

/**
 * @brief Construye un reconocedor de palabras a partir de un autómata de pila.
 *
 * @param automaton El autómata de pila.
 */
PushdownRecognizer::PushdownRecognizer(
    const PushdownAutomaton& automaton)
    : automaton_(automaton) {
}

/**
 * @brief Reconoce si una palabra pertenece al lenguaje del autómata de pila.
 *
 * @param word La palabra a reconocer.
 * @return Un objeto RecognitionResult que indica si la palabra es aceptada y proporciona información adicional.
 */
RecognitionResult PushdownRecognizer::recognize(
    const std::string& word) const {

    RecognitionResult result;

    for (const char symbol : word) {
        if (!automaton_.containsInputSymbol(symbol)) {
            result.inputValid = false;
            result.accepted = false;

            result.message =
                "La palabra contiene el símbolo '"
                + std::string(1, symbol)
                + "', que no pertenece al alfabeto de entrada Sigma.";

            return result;
        }
    }

    GrammarEngine engine(
        automaton_,
        word);

    if (engine.recognize(
            result.acceptingTransitionIndices)) {

        result.accepted = true;

        result.message =
            "La palabra pertenece al lenguaje reconocido por el APv.";
    } else {
        result.accepted = false;

        result.message =
            "La palabra no pertenece al lenguaje reconocido por el APv.";
    }

    return result;
}

}