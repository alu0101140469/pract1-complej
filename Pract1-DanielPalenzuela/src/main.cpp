/**
 * @file main.cpp
 * @brief Punto de entrada del simulador de autómata con pila APv.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "AutomatonParser.h"
#include "CommandLine.h"
#include "PushdownRecognizer.h"
#include "TraceWriter.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
           && std::isspace(
                  static_cast<unsigned char>(
                      value[begin]))) {
        ++begin;
    }

    std::size_t end = value.size();

    while (end > begin
           && std::isspace(
                  static_cast<unsigned char>(
                      value[end - 1]))) {
        --end;
    }

    return value.substr(
        begin,
        end - begin);
}

/**
 * @brief Lee palabras de un flujo de entrada, ignorando comentarios y líneas vacías.
 *
 * @param input Flujo de entrada del que se leerán las palabras.
 * @return Un vector de cadenas con las palabras leídas.
 */
std::vector<std::string> readWords(std::istream& input) {
    std::vector<std::string> words;
    std::string line;

    while (std::getline(input, line)) {
        const std::size_t comment =
            line.find('#');

        const std::string cleanLine =
            trim(line.substr(0, comment));

        if (cleanLine.empty()
            && comment != std::string::npos) {
            continue;
        }

        words.push_back(cleanLine);
    }

    return words;
}

std::string displayWord(
    const std::string& word) {
    return word.empty() ? "epsilon" : word;
}

}

/**
 * @brief Función principal del simulador de autómata con pila APv.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Vector de argumentos de línea de comandos.
 * @return Código de salida del programa.
 */
int main(
    int argc,
    char* argv[]) {

    try {
        const std::string programName =
            (argc > 0)
                ? argv[0]
                : "apv_simulator";

        const cc2627::ProgramOptions options =
            cc2627::CommandLine::parse(
                argc,
                argv,
                programName);

        const cc2627::PushdownAutomaton automaton =
            cc2627::AutomatonParser::parseFile(
                options.configFile);

        const cc2627::PushdownRecognizer recognizer(
            automaton);

        std::ifstream inputFile;

        std::istream* input = &std::cin;

        if (!options.inputFile.empty()) {
            inputFile.open(options.inputFile);

            if (!inputFile) {
                throw std::runtime_error(
                    "No se pudo abrir el fichero de entrada: "
                    + options.inputFile);
            }

            input = &inputFile;
        }

        std::ofstream traceFile;
        std::ostream* traceOutput = &std::cout;

        if (options.trace
            && !options.outputFile.empty()) {

            traceFile.open(options.outputFile);

            if (!traceFile) {
                throw std::runtime_error(
                    "No se pudo abrir el fichero de salida "
                    "de la traza: "
                    + options.outputFile);
            }

            traceOutput = &traceFile;
        }

        const std::vector<std::string> words =
            readWords(*input);

        if (words.empty()) {
            std::cerr
                << "No se han proporcionado cadenas de entrada.\n";

            return 1;
        }

        int errorCount = 0;

        // Procesa cada palabra de entrada y muestra el resultado.
        for (const std::string& word : words) {
            const cc2627::RecognitionResult result =
                recognizer.recognize(word);

            if (!result.inputValid) {
                std::cout
                    << displayWord(word)
                    << " -> ERROR: "
                    << result.message
                    << '\n';

                ++errorCount;
            } else {
                std::cout
                    << displayWord(word)
                    << " -> "
                    << (
                        result.accepted
                            ? "PERTENECE"
                            : "NO PERTENECE"
                    )
                    << '\n';
            }

            // Si la traza está activada, escribe la traza en el flujo de salida correspondiente.
            if (options.trace) {
                cc2627::TraceWriter::write(
                    automaton,
                    word,
                    result,
                    *traceOutput);
            }
        }

        return errorCount == 0 ? 0 : 2;

    } catch (const std::exception& error) {
        std::cerr
            << "Error: "
            << error.what()
            << '\n';

        return 1;
    }
}