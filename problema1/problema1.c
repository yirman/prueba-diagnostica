#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Función: esFENValida
 * ---------------------
 * Retorna 1 si la cadena 'fen' cumple con la notación FEN; de lo contrario retorna 0.
 */
int esFENValida(const char *fen) {
    char fenCopia[256];
    // Copiar la cadena de entrada (para no modificar el original)  
    strncpy(fenCopia, fen, 255);
    fenCopia[255] = '\0';

    // Dividir la cadena en 6 partes usando espacio como separador
    char *partes[6];
    int contadorPartes = 0;
    char *token = strtok(fenCopia, " ");
    while (token != NULL && contadorPartes < 6) {
        partes[contadorPartes++] = token;
        token = strtok(NULL, " ");
    }
    if (contadorPartes != 6) {
        return 0; // La notación FEN debe tener exactamente 6 partes
    }

    /* Validar la primera parte: Colocación de piezas (debe tener 8 filas separadas por '/')
       Se realiza una copia de partes[0] porque se va a dividir con strtok */
    char piezasCopia[256];
    strncpy(piezasCopia, partes[0], 255);
    piezasCopia[255] = '\0';
    
    char *filas[8];
    int contadorFilas = 0;
    token = strtok(piezasCopia, "/");
    while (token != NULL && contadorFilas < 8) {
        filas[contadorFilas++] = token;
        token = strtok(NULL, "/");
    }
    if (contadorFilas != 8) {
        return 0; // Debe haber 8 filas
    }
    
    // Para cada fila, contar casillas: dígitos suman su valor y cada pieza suma 1.
    for (int i = 0; i < 8; i++) {
        int sumaCasillas = 0;
        for (int j = 0; filas[i][j] != '\0'; j++){
            char c = filas[i][j];
            if (isdigit(c)) {
                sumaCasillas += (c - '0');
            } else if (strchr("prnbqkPRNBQK", c) != NULL) {
                sumaCasillas += 1;
            } else {
                return 0; // Carácter inválido en la colocación
            }
        }
        if (sumaCasillas != 8) {
            return 0; // Cada fila debe componer exactamente 8 casillas
        }
    }
    
    /* Validar la segunda parte: Color activo  
       Debe ser exactamente "w" o "b". */
    if (strcmp(partes[1], "w") != 0 && strcmp(partes[1], "b") != 0) {
        return 0;
    }
    
    /* Validar la tercera parte: Disponibilidad de enroque  
       Puede ser "-" o contener únicamente los caracteres 'K', 'Q', 'k', 'q'. */
    if (strcmp(partes[2], "-") != 0) {
        for (int i = 0; partes[2][i] != '\0'; i++) {
            char c = partes[2][i];
            if (c != 'K' && c != 'Q' && c != 'k' && c != 'q') {
                return 0;
            }
        }
    }
    
    /* Validar la cuarta parte: Casilla de captura al paso  
       Debe ser "-" o dos caracteres: letra entre a y h y dígito entre 1 y 8. */
    if (strcmp(partes[3], "-") != 0) {
        if (strlen(partes[3]) != 2)
            return 0;
        if (partes[3][0] < 'a' || partes[3][0] > 'h')
            return 0;
        if (partes[3][1] < '1' || partes[3][1] > '8')
            return 0;
    }
    
    /* Validar la quinta parte: Contador de semijugadas  
       Se verifica que todos sus caracteres sean dígitos. */
    for (int i = 0; partes[4][i] != '\0'; i++) {
        if (!isdigit(partes[4][i]))
            return 0;
    }
    
    /* Validar la sexta parte: Número total de jugadas  
       Se verifica que todos sus caracteres sean dígitos y su valor sea mayor que cero. */
    for (int i = 0; partes[5][i] != '\0'; i++) {
        if (!isdigit(partes[5][i]))
            return 0;
    }
    int numJugadas = atoi(partes[5]);
    if (numJugadas <= 0)
        return 0;
    
    return 1; // La cadena cumple con la notación FEN
}

int main() {
    // Ejemplos para probar la función
    const char *fenValido = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const char *fenInvalido = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR a KQkq - 0 1"; // Color activo incorrecto
    
    if (esFENValida(fenValido))
        printf("La cadena FEN '%s' es válida.\n", fenValido);
    else
        printf("La cadena FEN '%s' es inválida.\n", fenValido);
    
    if (esFENValida(fenInvalido))
        printf("La cadena FEN '%s' es válida.\n", fenInvalido);
    else
        printf("La cadena FEN '%s' es inválida.\n", fenInvalido);
    
    return 0;
}
