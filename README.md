# Top-Down y Bottom-Up Parsers
### Por Santiago Gomez Alzate
#
En computacion una CFG (Context Free Grammar) o Gramática Libre de Contexto es un tipo de gramática formal que se utiliza para describir la estructura sintáctica de un lenguaje formal. Es una herramienta utilizada en la teoría de lenguajes formales y en la compilación de programas

Una CFG consta de un conjunto de reglas de producción que definen cómo se pueden combinar diferentes símbolos para formar cadenas válidas en el lenguaje. Cada regla de producción consta de un símbolo no terminal (que puede ser reemplazado por otros símbolos) y una secuencia de símbolos terminales y no terminales.

Ejemplo:
* `S -> aSb | c`
  
`S` es el símbolo inicial
 
`S->aSb->aaSbb->aacbb` Ejemplo Derivación
 
`aacbb`, `acb`, `c`, `aaacbbb`... Pertenecen a la gramática

Para estas verificaciones existen analizadores sintacticos, dos de estos muy populares son Top Down y Bottom Up Parsers, descendiente y ascendente en funcionamiento respectivamente. Para lograr contruirlos se necesita analizar y extraer "claves" de la gramática, se usa el first y follow para ambos:
###### First: Para un símbolo no terminal es el conjunto de simbolos terminales posibles que al derivar el no terminal pueden resultar en primera posición del símbolo, puede estar epsilon
###### Follow: Para un símbolo no terminal es el conjunto de símbolos terminos posibles que pueden suceder a dicho símbolo no terminal
* Top-Down Parser: Con first y follow se puede contruir la tabla LL(1).Se puede usar para gramaticas LL1
* Bottom-Up Parser: Con first y follow se puede construir la tabla SLR.Se puede usar para gramáticas LR1
#### Notas:
##### 1: Ya que las gramáticas LR1 están incluidas en las LL1, si una gramática tiene Bottom-Up también será posible para esta el Top-Down
##### 2: Si durante el proceso de construcción del Top-Down o el Bottom-Up se encuentra alguna imposibilidad el proceso se detendrá y se dará por resultado que la gramática no es LL1 o LR1 respectivamente


### Convenciones:
* En el programa los simbolos no terminales son las letras mayúsculas y los terminales, son todos los demás símbolos a excepción de `^  [  ]  ,` 
* Epsilon se representa con `^`
* Para el .txt la gramatica se representa de la siguiente manera:
  * `NT[P1,P2,...PN,]`
  * Donde `NT` es el simbolo No Terminal y desde `P1` hasta `PN` las producciones
  #### Ejemplo:
    + Imagina la siguiente CFG
    + `S -> aSb | c | B`
    + `B -> b | ε(epsilon)`
    ####  La notación con las convenciones sería:
    +  `S[aSb,c,B,]`
    +  `B[b,^,]`
+ En el archivo de texto primero se pone la gramatica, luego en las líneas inmediatemente posteriores, se pueden poner las cadenas a analizar, Ej:
  ### archivo.txt
  #### S[aSb,c,B,] `Linea 1`
  #### B[b,^,] `Linea 2`
  #### acb `Linea 3 (Cadena Valida)`
  #### abb `Linea 4 (Cadena Valida)`
  #### aaacbbb`Linea 5 (Cadena Valida)`
  #### c`Linea 6 (Cadena Valida)`
  #### ... `(Lineas 7 a N-1)`
  #### baaac`Linea N (Cadena Invalida)`

### Intrucciones:
#### Luego de tener el archivo listo, cambia en el main() la ruta hacia el archivo txt, al ejecutar tendras las siguientes opciones
+ #### 1 - Opciones con top-down
  + ####  1 - First y Follow de la gramatica
  + #### 2 - First de una cadena
  + #### 3 - Tabla Top Down
  + #### 4 - Procesar las cadenas leidas del documento
  + #### 0 - Regresar
+ #### 2 - Opciones con bottom-up
  + #### 1 - Crear canonical con items
  + #### 2 - Imprimir tabla de analisis sintactico
  + #### 3 - Procesar las cadenas leidas del documento
  + #### 0 - Regresar
+ #### -1 - Salir

#### Puede elegir cualquier opción de los menús, sin importar el orden, solo habrá problema en caso de error de sintáxis del archivo o del tipo de gramática
  
    
