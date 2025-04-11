#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_STACK_SIZE 100
#define UMBRAL_CIENTIFICO_SUPERIOR 1e6
#define UMBRAL_CIENTIFICO_INFERIOR 1e-3

// Estructuras de pila (sin cambios)
typedef struct {
    double elementos[MAX_STACK_SIZE];
    int tope;
} PilaNumeros;

void inicializarPilaNumeros(PilaNumeros *p) {
    p->tope = -1;
}

void apilarNumero(PilaNumeros *p, double valor) {
    if (p->tope < MAX_STACK_SIZE - 1) {
        p->elementos[++(p->tope)] = valor;
    } else {
        fprintf(stderr, "Error: Desbordamiento de pila de números\n");
        exit(EXIT_FAILURE);
    }
}

double desapilarNumero(PilaNumeros *p) {
    if (p->tope >= 0) {
        return p->elementos[(p->tope)--];
    } else {
        fprintf(stderr, "Error: Subdesbordamiento de pila de números\n");
        exit(EXIT_FAILURE);
    }
}

int esPilaNumerosVacia(PilaNumeros *p) {
    return p->tope == -1;
}

typedef struct {
    char elementos[MAX_STACK_SIZE];
    int tope;
} PilaOperadores;

void inicializarPilaOperadores(PilaOperadores *p) {
    p->tope = -1;
}

void apilarOperador(PilaOperadores *p, char operador) {
    if (p->tope < MAX_STACK_SIZE - 1) {
        p->elementos[++(p->tope)] = operador;
    } else {
        fprintf(stderr, "Error: Desbordamiento de pila de operadores\n");
        exit(EXIT_FAILURE);
    }
}

char desapilarOperador(PilaOperadores *p) {
    if (p->tope >= 0) {
        return p->elementos[(p->tope)--];
    } else {
        fprintf(stderr, "Error: Subdesbordamiento de pila de operadores\n");
        exit(EXIT_FAILURE);
    }
}

char obtenerTopeOperador(PilaOperadores *p) {
    if (p->tope >= 0) {
        return p->elementos[p->tope];
    } else {
        return '\0'; // Indica pila vacía
    }
}

int esPilaOperadoresVacia(PilaOperadores *p) {
    return p->tope == -1;
}

// Función para obtener la precedencia de un operador (sin cambios)
int obtenerPrecedencia(char operador) {
    switch (operador) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

// Función para aplicar una operación (sin cambios)
void aplicarOperacion(PilaOperadores *operadores, PilaNumeros *valores) {
    char operador = desapilarOperador(operadores);
    double operando2 = desapilarNumero(valores);
    double operando1 = desapilarNumero(valores);
    switch (operador) {
        case '+':
            apilarNumero(valores, operando1 + operando2);
            break;
        case '-':
            apilarNumero(valores, operando1 - operando2);
            break;
        case '*':
            apilarNumero(valores, operando1 * operando2);
            break;
        case '/':
            if (operando2 == 0.0) {
                fprintf(stderr, "Error: División por cero\n");
                exit(EXIT_FAILURE);
            }
            apilarNumero(valores, operando1 / operando2);
            break;
    }
}

// Función para analizar un número, incluyendo notación científica (sin cambios)
double analizarNumero(const char *cadena, int *indice) {
    double valor = 0.0;
    double signo = 1.0;
    double decimal = 0.0;
    int exponente = 0;
    int signoExponente = 1;
    int valorExponente = 0;
    int lugaresDecimales = 0;

    if (cadena[*indice] == '-') {
        signo = -1.0;
        (*indice)++;
    }

    while (isdigit(cadena[*indice])) {
        valor = valor * 10 + (cadena[*indice] - '0');
        (*indice)++;
    }

    if (cadena[*indice] == '.') {
        (*indice)++;
        while (isdigit(cadena[*indice])) {
            decimal = decimal * 10 + (cadena[*indice] - '0');
            lugaresDecimales++;
            (*indice)++;
        }
        valor += decimal / pow(10, lugaresDecimales);
    }

    if (toupper(cadena[*indice]) == 'E') {
        (*indice)++;
        if (cadena[*indice] == '-') {
            signoExponente = -1;
            (*indice)++;
        } else if (cadena[*indice] == '+') {
            (*indice)++;
        }
        while (isdigit(cadena[*indice])) {
            valorExponente = valorExponente * 10 + (cadena[*indice] - '0');
            (*indice)++;
        }
        exponente = signoExponente * valorExponente;
        valor *= pow(10, exponente);
    }

    return signo * valor;
}

//Basado en algoritmo Shunting yard, método común para analizar y evaluar expresiones aritméticas
double evaluarExpresion(const char *expresion) {
    PilaNumeros valores;
    PilaOperadores operadores;
    inicializarPilaNumeros(&valores);
    inicializarPilaOperadores(&operadores);

    int i = 0;
    while (expresion[i] != '\0') {
        if (isspace(expresion[i])) {    //Manejo de espacios en blanco
            i++;
            continue;
        } else if (isdigit(expresion[i]) || expresion[i] == '-' && (i == 0 || expresion[i-1] == '(' || obtenerPrecedencia(expresion[i-1]) > 0) || expresion[i] == '.') { //Manejo de números
            int indiceActual = i;
            apilarNumero(&valores, analizarNumero(expresion, &i));
            i = indiceActual;
            while (isdigit(expresion[i]) || expresion[i] == '.' || toupper(expresion[i]) == 'E' || (expresion[i] == '-' && toupper(expresion[i-1]) == 'E') || (expresion[i] == '+' && toupper(expresion[i-1]) == 'E')) {
                i++;
            }
            continue;
        } else if (expresion[i] == '(') {                   //Manejo de paréntesis izquierdo
            apilarOperador(&operadores, expresion[i]);
            i++;
        } else if (expresion[i] == ')') {                   //Manejo de paréntesis derecho
            while (!esPilaOperadoresVacia(&operadores) && obtenerTopeOperador(&operadores) != '(') {
                aplicarOperacion(&operadores, &valores);
            }
            if (!esPilaOperadoresVacia(&operadores) && obtenerTopeOperador(&operadores) == '(') {
                desapilarOperador(&operadores); // Desapila el '('
            } else {
                fprintf(stderr, "Error: Paréntesis desbalanceados\n");
                exit(EXIT_FAILURE);
            }
            i++;
        } else if (expresion[i] == '+' || expresion[i] == '-' || expresion[i] == '*' || expresion[i] == '/') {          //Manejo de operadores
            while (!esPilaOperadoresVacia(&operadores) && obtenerTopeOperador(&operadores) != '(' && obtenerPrecedencia(obtenerTopeOperador(&operadores)) >= obtenerPrecedencia(expresion[i])) {
                aplicarOperacion(&operadores, &valores);
            }
            apilarOperador(&operadores, expresion[i]);
            i++;
        } else {
            fprintf(stderr, "Error: Carácter inválido en la expresión: %c\n", expresion[i]);
            exit(EXIT_FAILURE);
        }
    }

    while (!esPilaOperadoresVacia(&operadores)) {
        if (obtenerTopeOperador(&operadores) == '(' || obtenerTopeOperador(&operadores) == ')') {
            fprintf(stderr, "Error: Paréntesis desbalanceados\n");
            exit(EXIT_FAILURE);
        }
        aplicarOperacion(&operadores, &valores);
    }

    if (!esPilaNumerosVacia(&valores) && valores.tope == 0) {
        return desapilarNumero(&valores);
    } else {
        fprintf(stderr, "Error: Expresión inválida\n");
        exit(EXIT_FAILURE);
    }
}

// Función para formatear un número en notación científica si es necesario
void formatearCientifico(double resultado) {
    if (fabs(resultado) > UMBRAL_CIENTIFICO_SUPERIOR || (fabs(resultado) < UMBRAL_CIENTIFICO_INFERIOR && fabs(resultado) > 0)) {
        printf("Resultado: %e\n", resultado);
    } else {
        printf("Resultado: %lf\n", resultado);
    }
}

int main() {
    char expression[] = "(125E10 - 1e15)/5E-85*15";
    double result = evaluarExpresion(expression);
    printf("Resultado de '%s': %lf\n", expression, result);
    formatearCientifico(result);

    return 0;
}