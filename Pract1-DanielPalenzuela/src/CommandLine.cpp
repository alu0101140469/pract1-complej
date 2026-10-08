/**
 * @file CommandLine.cpp
 * @brief Implementa el analizador de opciones de línea de comandos.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include "CommandLine.h"

#include <sstream>
#include <stdexcept>

namespace cc2627 {

/**
 * @brief Parsea las opciones de línea de comandos y devuelve un objeto ProgramOptions.
 *
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos.
 * @param programName Nombre del programa (argv[0]).
 * @return Un objeto ProgramOptions con las opciones parseadas.
 */
ProgramOptions CommandLine::parse(
    int argc,
    char* argv[],
    const std::string& programName) {

    ProgramOptions options;
    bool hasConfig = false;
    bool hasTrace = false;

    if (argc == 2
        && (std::string(argv[1]) == "-h"
            || std::string(argv[1]) == "--help")) {
        throw std::runtime_error(usage(programName));
    }

    for (int index = 1; index < argc; ++index) {
        const std::string option = argv[index];

        // Valida y procesa cada opción de línea de comandos.
        if (option == "-config") {
            if (index + 1 >= argc) {
                throw std::runtime_error(
                    "La opción -config requiere un fichero.\n\n"
                    + usage(programName));
            }

            options.configFile = argv[++index];
            hasConfig = true;
        } else if (option == "-trace") {
            if (index + 1 >= argc) {
                throw std::runtime_error(
                    "La opción -trace requiere y o n.\n\n"
                    + usage(programName));
            }

            const std::string value = argv[++index];

            if (value != "y" && value != "n") {
                throw std::runtime_error(
                    "El valor de -trace debe ser y o n.\n\n"
                    + usage(programName));
            }

            options.trace = (value == "y");
            hasTrace = true;
        } else if (option == "-in") {
            if (index + 1 >= argc) {
                throw std::runtime_error(
                    "La opción -in requiere un fichero.\n\n"
                    + usage(programName));
            }

            options.inputFile = argv[++index];
        } else if (option == "-out") {
            if (index + 1 >= argc) {
                throw std::runtime_error(
                    "La opción -out requiere un fichero.\n\n"
                    + usage(programName));
            }

            options.outputFile = argv[++index];
        } else if (option == "-h" || option == "--help") {
            throw std::runtime_error(usage(programName));
        } else {
            throw std::runtime_error(
                "Opción desconocida: " + option + "\n\n"
                + usage(programName));
        }
    }

    if (!hasConfig || !hasTrace) {
        throw std::runtime_error(
            "Son obligatorias las opciones -config y -trace.\n\n"
            + usage(programName));
    }

    if (!options.trace && !options.outputFile.empty()) {
        throw std::runtime_error(
            "La opción -out solo se puede utilizar cuando -trace y.");
    }

    return options;
}

/**
 * @brief Devuelve un mensaje de uso del programa.
 *
 * @param programName Nombre del programa (argv[0]).
 * @return Un string con el mensaje de uso.
 */
std::string CommandLine::usage(const std::string& programName) {
    std::ostringstream out;

    out << "Uso:\n"
        << "  " << programName
        << " -config <f> -trace <y|n> -in <f> [-out <f>]\n\n"
        << "Opciones:\n"
        << "  -config <f>  Fichero con la definición del APv.\n"
        << "  -trace <y|n> Activa o desactiva la traza.\n"
        << "  -in <f>      Fichero con una cadena por línea.\n"
        << "  -out <f>     Fichero donde se escribe la traza y solo con -trace y.\n"
        << "  -h, --help   Muestra esta ayuda.\n";

    return out.str();
}

}