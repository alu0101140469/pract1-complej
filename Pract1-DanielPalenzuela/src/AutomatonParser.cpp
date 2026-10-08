/**
 * @file AutomatonParser.cpp
 * @brief Implementa la lectura y validación del fichero de configuración.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "AutomatonParser.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cc2627 {
namespace {

/**
 * @brief Elimina los espacios en blanco al inicio y al final de una cadena.
 *
 * @param value La cadena a limpiar.
 * @return La cadena sin espacios en blanco al inicio y al final.
 */
std::string trim(const std::string& value) {
    std::size_t begin = 0;

    while (begin < value.size()
           && std::isspace(static_cast<unsigned char>(value[begin]))) {
        ++begin;
    }

    std::size_t end = value.size();

    while (end > begin
           && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    return value.substr(begin, end - begin);
}

/**
 * @brief Elimina los comentarios de una línea.
 *
 * @param line La línea de la que se eliminará el comentario.
 * @return La línea sin el comentario.
 */
std::string removeComment(const std::string& line) {
    const std::size_t comment = line.find('#');
    return trim(line.substr(0, comment));
}

/**
 * @brief Tokeniza una línea en palabras separadas por espacios.
 *
 * @param line La línea a tokenizar.
 * @return Un vector de palabras tokenizadas.
 */
std::vector<std::string> tokenize(const std::string& line) {
    std::istringstream input(line);
    std::vector<std::string> tokens;
    std::string token;

    while (input >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

/**
 * @brief Lanza una excepción de error de configuración con un mensaje detallado.
 *
 * @param filename El nombre del fichero de configuración.
 * @param lineNumber El número de línea donde ocurrió el error.
 * @param message El mensaje de error.
 */
[[noreturn]] void configurationError(const std::string& filename,
                                     std::size_t lineNumber,
                                     const std::string& message) {
    std::ostringstream out;
    out << filename << ':' << lineNumber << ": " << message;
    throw std::runtime_error(out.str());
}

/**
 * @brief Parsea un token que representa un símbolo y valida su formato.
 *
 * @param token El token a parsear.
 * @param filename El nombre del fichero de configuración.
 * @param lineNumber El número de línea donde se encuentra el token.
 * @param context El contexto del token (por ejemplo, "Sigma" o "Gamma").
 * @return El carácter representado por el token.
 */
char parseSingleSymbol(const std::string& token,
                       const std::string& filename,
                       std::size_t lineNumber,
                       const std::string& context) {
    if (token.size() != 1 || token == ".") {
        configurationError(
            filename,
            lineNumber,
            context
                + " debe contener exactamente un carácter distinto de '.'.");
    }

    return token.front();
}

}

/**
 * @brief Parsea un fichero de configuración y construye un autómata de pila.
 *
 * @param filename El nombre del fichero de configuración.
 * @return Un objeto PushdownAutomaton construido a partir del fichero.
 */
PushdownAutomaton AutomatonParser::parseFile(const std::string& filename) {
    std::ifstream input(filename);

    if (!input) {
        throw std::runtime_error(
            "No se pudo abrir el fichero de configuración: " + filename);
    }

    std::vector<std::pair<std::size_t, std::vector<std::string>>> content;
    std::string line;
    std::size_t lineNumber = 0;

    // Lee el fichero línea por línea, eliminando comentarios y tokenizando.
    while (std::getline(input, line)) {
        ++lineNumber;

        const std::string cleanLine = removeComment(line);

        if (!cleanLine.empty()) {
            content.emplace_back(lineNumber, tokenize(cleanLine));
        }
    }

    // Valida que el fichero tenga al menos cinco líneas de definición para un APv.
    if (content.size() < 5) {
        throw std::runtime_error(
            "El fichero debe contener al menos cinco líneas de definición "
            "para un APv: Q, Sigma, Gamma, estado inicial y símbolo inicial "
            "de pila.");
    }

    // Valida y construye los conjuntos de estados, alfabeto de entrada y alfabeto de pila.
    const auto& statesTokens = content[0].second;

    // Valida que el conjunto de estados no esté vacío y no contenga duplicados.
    if (statesTokens.empty()) {
        configurationError(
            filename,
            content[0].first,
            "El conjunto Q no puede estar vacío.");
    }

    // Valida que no haya estados repetidos en el conjunto de estados.
    std::set<std::string> states(
        statesTokens.begin(),
        statesTokens.end());

    if (states.size() != statesTokens.size()) {
        configurationError(
            filename,
            content[0].first,
            "El conjunto Q no puede contener estados repetidos.");
    }

    const auto& inputTokens = content[1].second;
    std::set<char> inputAlphabet;

    // Valida que el alfabeto de entrada no esté vacío y no contenga duplicados.
    for (const std::string& token : inputTokens) {
        if (token == ".") {
            configurationError(
                filename,
                content[1].first,
                "El símbolo '.' está reservado para epsilon "
                "y no puede pertenecer al alfabeto de entrada.");
        }

        inputAlphabet.insert(
            parseSingleSymbol(
                token,
                filename,
                content[1].first,
                "Cada símbolo de Sigma"));
    }

    const auto& stackTokens = content[2].second;

    // Valida que el alfabeto de pila no esté vacío y no contenga duplicados.
    if (stackTokens.empty()) {
        configurationError(
            filename,
            content[2].first,
            "El alfabeto de pila Gamma no puede estar vacío.");
    }

    std::set<char> stackAlphabet;

    // Valida que no haya símbolos repetidos en el alfabeto de pila.
    for (const std::string& token : stackTokens) {
        if (token == ".") {
            configurationError(
                filename,
                content[2].first,
                "El símbolo '.' está reservado para epsilon "
                "y no puede pertenecer al alfabeto de pila.");
        }

        stackAlphabet.insert(
            parseSingleSymbol(
                token,
                filename,
                content[2].first,
                "Cada símbolo de Gamma"));
    }

    if (content[3].second.size() != 1) {
        configurationError(
            filename,
            content[3].first,
            "El estado inicial debe ser un único estado de Q.");
    }

    const std::string initialState = content[3].second.front();

    // Valida que el estado inicial pertenezca al conjunto de estados definidos.
    if (states.find(initialState) == states.end()) {
        configurationError(
            filename,
            content[3].first,
            "El estado inicial debe pertenecer a Q.");
    }

    // Valida que el símbolo inicial de la pila sea un único símbolo del alfabeto de pila.
    if (content[4].second.size() != 1) {
        configurationError(
            filename,
            content[4].first,
            "El símbolo inicial de la pila debe ser un único símbolo de Gamma.");
    }

    // Valida que el símbolo inicial de la pila pertenezca al alfabeto de pila definido.
    const char initialStackSymbol = parseSingleSymbol(
        content[4].second.front(),
        filename,
        content[4].first,
        "El símbolo inicial de la pila");

    if (stackAlphabet.find(initialStackSymbol) == stackAlphabet.end()) {
        configurationError(
            filename,
            content[4].first,
            "El símbolo inicial de la pila debe pertenecer a Gamma.");
    }

    std::vector<Transition> transitions;

    // Valida y construye las transiciones del autómata de pila.
    for (std::size_t index = 5; index < content.size(); ++index) {
        const std::size_t currentLine = content[index].first;
        const std::vector<std::string>& tokens = content[index].second;

        if (tokens.size() != 5) {
            configurationError(
                filename,
                currentLine,
                "Una transición de APv debe tener exactamente cinco "
                "campos: estado, entrada, cima, estado destino y reposición.");
        }

        const std::string& sourceState = tokens[0];
        const std::string& inputToken = tokens[1];

        const char stackTop = parseSingleSymbol(
            tokens[2],
            filename,
            currentLine,
            "El símbolo de cima de una transición");

        const std::string& destinationState = tokens[3];

        if (states.find(sourceState) == states.end()) {
            configurationError(
                filename,
                currentLine,
                "El estado origen de la transición debe pertenecer a Q.");
        }

        if (states.find(destinationState) == states.end()) {
            configurationError(
                filename,
                currentLine,
                "El estado destino de la transición debe pertenecer a Q.");
        }

        if (stackAlphabet.find(stackTop) == stackAlphabet.end()) {
            configurationError(
                filename,
                currentLine,
                "El símbolo de cima de una transición debe pertenecer a Gamma.");
        }

        std::optional<char> inputSymbol;

        if (inputToken == ".") {
            inputSymbol = std::nullopt;
        } else {
            inputSymbol = parseSingleSymbol(
                inputToken,
                filename,
                currentLine,
                "El símbolo de entrada de una transición");

            if (inputAlphabet.find(*inputSymbol) == inputAlphabet.end()) {
                configurationError(
                    filename,
                    currentLine,
                    "El símbolo de entrada de la transición debe pertenecer "
                    "a Sigma o ser '.'.");
            }
        }

        std::string replacement;

        if (tokens[4] != ".") {
            replacement = tokens[4];

            for (const char symbol : replacement) {
                if (symbol == '.'
                    || stackAlphabet.find(symbol) == stackAlphabet.end()) {
                    configurationError(
                        filename,
                        currentLine,
                        "La reposición de pila debe ser '.' o una cadena "
                        "no vacía de símbolos de Gamma.");
                }
            }
        }

        // Valida que la transición no sea una transición epsilon que no modifique la pila.
        transitions.emplace_back(
            sourceState,
            inputSymbol,
            stackTop,
            destinationState,
            std::move(replacement));
    }

    // Construye y devuelve el autómata de pila con los elementos validados.
    return PushdownAutomaton(
        std::move(states),
        std::move(inputAlphabet),
        std::move(stackAlphabet),
        initialState,
        initialStackSymbol,
        std::move(transitions));
}

}