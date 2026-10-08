#ifndef CC2627_COMMAND_LINE_H
#define CC2627_COMMAND_LINE_H

/**
 * @file CommandLine.h
 * @brief Declara el analizador de opciones de línea de comandos.
 *
 * Proyecto: Complejidad Computacional - Práctica 1
 * Curso: 2026/27
 * Tipo de autómata implementado: APv (vaciado de pila)
 * @author Daniel Palenzuela Álvarez alu0101140469
 */

#include <string>

namespace cc2627 {

/**
 * @struct ProgramOptions
 * @brief Almacena las opciones de ejecución del programa principal.
 */
struct ProgramOptions {
    std::string configFile;
    bool trace = false;
    std::string inputFile;
    std::string outputFile;
};

/**
 * @class CommandLine
 * @brief Valida y convierte los argumentos de línea de comandos en opciones de ejecución.
 */
class CommandLine {
public:
    /**
     * @brief Parsea la línea de comandos.
     * @param argc Número de argumentos.
     * @param argv Argumentos del proceso.
     * @param programName Nombre del ejecutable para mensajes de ayuda.
     * @return Opciones validadas.
     */
    static ProgramOptions parse(int argc, char* argv[], const std::string& programName);

    /**
     * @brief Devuelve el texto de ayuda del programa.
     * @param programName Nombre del ejecutable.
     * @return Ayuda de uso.
     */
    static std::string usage(const std::string& programName);
};

}

#endif