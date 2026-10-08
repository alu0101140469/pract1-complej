# Práctica 1 - Simulador de un Autómata con Pila (APv)

## 1. Información del proyecto

* **Alumno:** Daniel Palenzuela Álvarez alu0101140469
* **Asignatura:** Complejidad Computacional
* **Curso:** 2026/27
* **Práctica 1:** Programar un simulador de un autómata con pila
* **Tipo de autómata implementado:** Autómata con Pila por vaciado de pila (**APv**)


El objetivo del proyecto es implementar un simulador de un autómata con pila siguiendo el formato de configuración indicado en la práctica. El programa permite cargar un APv desde un fichero, validar su definición, comprobar varias cadenas de entrada y, opcionalmente, generar una traza de la ejecución.

La práctica permite escoger entre un autómata con pila por vaciado de pila (APv) o por estado final (APf). En este proyecto se ha elegido **APv**, por lo que una palabra se considera aceptada cuando existe una ejecución que consume toda la entrada y consigue dejar la pila vacía.

---

## 2. ¿Qué hace el programa?

El programa funciona como un simulador de un Autómata con Pila por vaciado de pila.

```text
          Fichero de configuración
                    |
                    v
             AutomatonParser
                    |
                    v
          PushdownAutomaton
                    |
                    v
         PushdownRecognizer
                    |
             ¿aceptada?
              /       \
            sí         no
            |           |
            v           v
       PERTENECE    NO PERTENECE
            |
            v
        TraceWriter
```

El usuario proporciona:

1. Un fichero con la definición del APv.
2. La opción de activar o desactivar la traza.
3. Un fichero con las palabras que se quieren comprobar.
4. Un fichero donde guardar la traza.

Para cada palabra, el programa comprueba si pertenece al lenguaje reconocido por el autómata y muestra si pertenece o no pertenece. Cuando la traza está activada, además se muestran las configuraciones del autómata durante la ejecución.

---

# 3. Formato del fichero de configuración

La definición del autómata se introduce en un fichero de texto siguiendo el formato indicado en la práctica:

```text
# Comentarios
q1 q2 q3 ...       # conjunto Q
a1 a2 a3 ...       # conjunto Sigma
A1 A2 A3 ...       # conjunto Gamma
q1                 # estado inicial
A1                 # símbolo inicial de la pila
q1 a A1 q2 A       # transición
...
```

Para un APv se utilizan:

* **Q:** conjunto de estados.
* **Sigma:** alfabeto de entrada.
* **Gamma:** alfabeto de pila.
* **Estado inicial.**
* **Símbolo inicial de la pila.**
* **Transiciones.**

Una transición tiene cinco campos:

```text
estado_origen simbolo_entrada cima_pila estado_destino reposicion_pila
```

Por ejemplo:

```text
q1 a A q1 AA
```

representa una transición en la que el estado actual es `q1`, se consume `a`,la cima de la pila debe ser `A`, el nuevo estado es `q1` y el símbolo `A` que estaba en la cima se sustituye por `AA`.

### Representación de epsilon

El carácter `.` representa **epsilon** en los lugares donde lo permite el formato del fichero.

Por ejemplo:

```text
q1 . S q2 S
```

significa que la transición no consume ningún símbolo de entrada.

Y en este otro caso:

```text
q2 b A q2 .
```

significa que se consume `b` y se elimina `A` de la pila.

El `.` está reservado para representar epsilon, por lo que **no puede pertenecer al alfabeto de entrada ni al alfabeto de pila**.

---

# 4. Validación de la configuración

Antes de ejecutar el autómata, el programa comprueba que el fichero contiene una definición correcta.

Se verifica que:

* el conjunto de estados no esté vacío
* el estado inicial pertenezca a `Q`
* el símbolo inicial de la pila pertenezca a `Gamma`
* los símbolos de `Sigma` tengan un único carácter
* los símbolos de `Gamma` tengan un único carácter
* `.` no aparezca como símbolo de `Sigma` ni de `Gamma`
* los estados utilizados por las transiciones pertenezcan a `Q`
* el símbolo de cima de una transición pertenezca a `Gamma`
* el símbolo de entrada de una transición pertenezca a `Sigma` o sea `.`
* la reposición de la pila sea `.` o una cadena formada únicamente por símbolos de `Gamma`

Los comentarios del fichero comienzan en `#` y se ignoran.

---

# 5. Estructura del proyecto

El proyecto está organizado separando la interfaz, la implementación, las pruebas y los ficheros generados durante la compilación.

```text
APvPractica1/
├── src/
│   ├── AutomatonParser.cpp
│   ├── CommandLine.cpp
│   ├── Configuration.cpp
│   ├── PushdownAutomaton.cpp
│   ├── PushdownRecognizer.cpp
│   ├── TraceWriter.cpp
│   ├── Transition.cpp
│   └── main.cpp
│
├── include/
│       ├── AutomatonParser.h
│       ├── CommandLine.h
│       ├── Configuration.h
│       ├── PushdownAutomaton.h
│       ├── PushdownRecognizer.h
│       ├── TraceWriter.h
│       └── Transition.h
│
├── build/
│   └── apv_simulator
│
├── tests/
│   ├── APv/
│   │   ├── APv-1.txt
│   │   ├── APv-2.txt
│   │   └── APv-3.txt
│   ├── output/
│   │   ├── trace_apv1.txt
│   │   ├── trace_apv2.txt
│   │   └── trace_apv3.txt
│   ├── input/
│   │   ├── apv1_words.txt
│   │   ├── apv2_words.txt
│   │   └── apv3_words.txt
│
├── Makefile
└── README.md
```

---

# 6. Descripción de los ficheros de código

## 6.1. `src/main.cpp`

`main.cpp` es el programa principal y se encarga de coordinar todos los componentes.

1. Procesar los argumentos de línea de comandos mediante `CommandLine`.
2. Leer el fichero de configuración mediante `AutomatonParser`.
3. Crear el `PushdownRecognizer`.
4. Obtener las palabras desde un fichero.
5. Ejecutar el reconocimiento para cada palabra.
6. Mostrar el resultado.
7. Generar la traza mediante `TraceWriter` cuando se ha solicitado.

---

## 6.2. `include/CommandLine.h` y `src/CommandLine.cpp`

La clase `CommandLine` se encarga de interpretar y validar las opciones utilizadas al ejecutar el programa. Esta clase permite que el tratamiento de argumentos esté separado del resto de la lógica del programa. Las opciones principales son:

```text
-config <f>
-trace <y|n>
[-in <f>]
[-out <f>]
```

Utiliza la estructura `ProgramOptions`, que almacena:

```cpp
std::string configFile;
bool trace;
std::string inputFile;
std::string outputFile;
```

El método:

```cpp
CommandLine::parse(...)
```

recorre los argumentos, comprueba que las opciones sean correctas y devuelve una estructura `ProgramOptions`.

También se comprueba que:

* `-config` tenga un fichero asociado
* `-trace` sea `y` o `n`
* las opciones desconocidas produzcan un error
* `-out` no se utilice si la traza está desactivada

El método `usage(...)` genera el texto de ayuda.

---

## 6.3. `include/Transition.h` y `src/Transition.cpp`

La clase `Transition` representa una transición individual del autómata.

Sus datos principales son:

```cpp
std::string sourceState_;
std::optional<char> inputSymbol_;
char stackTop_;
std::string destinationState_;
std::string replacement_;
```

Estos atributos representan:

* estado origen
* símbolo de entrada
* símbolo que debe estar en la cima de la pila
* estado destino
* cadena que sustituye a la cima

Se utiliza:

```cpp
std::optional<char>
```

para representar el símbolo de entrada. Si contiene un carácter, la transición consume ese carácter. Si contiene `std::nullopt`, significa que es una transición epsilon y no consume entrada.

El método:

```cpp
consumesEpsilon()
```

indica si la transición es epsilon.

Finalmente:

```cpp
toString()
```

devuelve la transición en un formato legible y se utiliza principalmente para mostrarla en la traza.

---

## 6.4. `include/Configuration.h` y `src/Configuration.cpp`

La clase `Configuration` representa una configuración directa del autómata. Esta clase se utiliza especialmente durante la aplicación de transiciones y para construir la información que se muestra en la traza. Una configuración está formada por:

```text
(estado, cadena restante, pila)
```

La clase contiene:

```cpp
std::string state_;
std::string remainingInput_;
std::string stack_;
```

La pila se almacena con la cima en la primera posición.

Por ejemplo:

```text
AAS
```

significa que la cima de la pila es `A`.

El método:

```cpp
isStackEmpty()
```

permite comprobar si la pila está vacía.

---

## 6.5. `include/PushdownAutomaton.h` y `src/PushdownAutomaton.cpp`

`PushdownAutomaton` representa el autómata completo.

Contiene:

```cpp
std::set<std::string> states_;
std::set<char> inputAlphabet_;
std::set<char> stackAlphabet_;
std::string initialState_;
char initialStackSymbol_;
std::vector<Transition> transitions_;
```

Por tanto, almacena los componentes principales del APv:

```text
Q
Sigma
Gamma
estado inicial
símbolo inicial de pila
función de transición
```

Además de ofrecer métodos para consultar estos datos, destacan dos operaciones:

### `getApplicableTransitionIndices(...)`

Recibe una `Configuration` y decide qué transiciones se pueden aplicar. Para que una transición sea aplicable se comprueba:

1. que el estado actual coincida con el estado origen
2. que el símbolo de la cima de la pila coincida
3. que la transición sea epsilon o consuma el siguiente símbolo de entrada

Este método devuelve los índices de **todas** las transiciones aplicables. Esto es importante porque un autómata de pila puede ser no determinista y puede tener varias transiciones posibles desde una misma configuración.

### `applyTransition(...)`

Este método aplica una transición a una configuración. Cuando la transición consume entrada, elimina el primer carácter de la cadena restante, y después elimina el símbolo que estaba en la cima de la pila y coloca en su lugar la cadena de reposición indicada por la transición.

Por ejemplo, si:

```text
pila = AXYZ
reposición = BC
```

la nueva pila será:

```text
BCXYZ
```

De esta forma esta clase contiene la representación y las operaciones básicas del autómata, pero no decide por sí sola si una palabra pertenece al lenguaje.

---

## 6.6. `include/AutomatonParser.h` y `src/AutomatonParser.cpp`

`AutomatonParser` se encarga de leer el fichero de configuración y convertir su contenido en un objeto `PushdownAutomaton`. El método principal es:

```cpp
AutomatonParser::parseFile(...)
```

El proceso es:

1. Abrir el fichero.
2. Leer las líneas.
3. Eliminar comentarios y espacios innecesarios.
4. Interpretar `Q`, `Sigma`, `Gamma`, estado inicial y símbolo inicial.
5. Leer las transiciones.
6. Validar todos los datos.
7. Construir y devolver el `PushdownAutomaton`.

---

## 6.7. `include/PushdownRecognizer.h` y `src/PushdownRecognizer.cpp`

Esta es la clase encargada de decidir si una palabra pertenece al lenguaje del APv. El método principal es:

```cpp
RecognitionResult recognize(const std::string& word)
```

Antes de realizar el reconocimiento, se comprueba que todos los símbolos de la palabra pertenezcan a `Sigma`, y después se realiza el reconocimiento teniendo en cuenta el posible **no determinismo** del APv.  
Un AP puede tener varias transiciones posibles, por lo que no se puede escoger directamente una transición, porque una rama podría fallar y otra aceptar la palabra.

El programa utiliza `GrammarEngine`, que construye una representación equivalente basada en variables que representan la posibilidad de eliminar un símbolo de pila pasando de un estado a otro y consumiendo una determinada parte de la palabra. A partir de las transiciones del APv se generan producciones y se utiliza una tabla de reconocimiento (`chart_`) para almacenar los intervalos de entrada que pueden ser reconocidos por esas variables. Este procedimiento permite tratar con transiciones no deterministas, transiciones epsilon, transiciones que introducen varios símbolos en la pila o ciclos de transiciones epsilon.

Cuando se encuentra una derivación aceptada, el código conserva un `Witness`, es decir, información que permite reconstruir las decisiones que llevaron a esa aceptación.

Finalmente, `reconstruct(...)` transforma esa información en una secuencia de índices de transiciones. Esa secuencia es la que usará `TraceWriter`.

---

## 6.8. `RecognitionResult`

`RecognitionResult` es la estructura que contiene el resultado.

```cpp
bool accepted;
bool inputValid;
std::string message;
std::vector<std::size_t> acceptingTransitionIndices;
```

Permite devolver de una sola vez si la palabra es válida, si pertenece al lenguaje, un mensaje explicativo y el camino aceptante si existe. Esto mantiene separada la lógica de reconocimiento de la salida por pantalla.

---

## 6.9. `include/TraceWriter.h` y `src/TraceWriter.cpp`

`TraceWriter` es responsable de generar la salida del modo traza. Recibe el autómata, la palabra, el resultado del reconocimiento y el flujo de salida.

Comienza con la configuración inicial:

```text
(estado inicial, palabra completa, símbolo inicial de pila)
```

Después recorre las transiciones guardadas en:

```cpp
acceptingTransitionIndices
```

Para cada transición utiliza:

```cpp
automaton.applyTransition(...)
```

y obtiene la nueva configuración.

La traza muestra, después de cada transición:

```text
Estado
Cadena restante
Pila
Transiciones posibles
```

Además, como trabaja con `std::ostream`, puede escribir tanto en:

```cpp
std::cout
```

como en un:

```cpp
std::ofstream
```

Esto permite implementar la opción `-out`.

---

# 7. Ejecución

### Paso 1: argumentos

El usuario ejecuta el programa:

```bash
./build/apv_simulator -config fichero.txt -trace y -in palabras.txt
```

`CommandLine` interpreta las opciones.

### Paso 2: lectura del APv

`AutomatonParser` lee el fichero y construye el `PushdownAutomaton`.

### Paso 3: validación

Si existe un error en la definición, el programa termina mostrando un mensaje de error.

### Paso 4: lectura de palabras

Las palabras se leen desde el fichero indicado con `-in`.

### Paso 5: reconocimiento

Para cada palabra se llama a:

```cpp
recognizer.recognize(word);
```

### Paso 6: resultado

El programa muestra:

```text
palabra -> PERTENECE
```

o:

```text
palabra -> NO PERTENECE
```

### Paso 7: traza

Si `-trace y`, `TraceWriter` genera la secuencia de configuraciones correspondiente al camino aceptante en un fichero de salida.

# 8. Modo traza

El programa dispone de:

```text
-trace y
```

para activar la traza, y:

```text
-trace n
```

para desactivarla.

Cuando está activa, la traza incluye información sobre:

* estado actual
* cadena restante
* contenido de la pila
* transiciones aplicables
* transición aplicada en cada paso
* resultado final

La traza puede mostrarse por pantalla o guardarse en un fichero mediante:

```text
-out <f>
```

---