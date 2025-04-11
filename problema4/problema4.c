#include <stdio.h>
#include <string.h>

#define MAX_TEXT_LENGTH 1024
#define MAX_WORD_LENGTH 100

int main(void) {
    char texto[MAX_TEXT_LENGTH];
    char palabra[MAX_WORD_LENGTH];
    
    // Solicitar la cadena de caracteres
    printf("Ingrese la cadena:\n");
    if (fgets(texto, sizeof(texto), stdin) == NULL) {
        fprintf(stderr, "Error al leer la cadena.\n");
        return 1;
    }
    // Eliminar el salto de línea final
    size_t len = strlen(texto);
    if (len > 0 && texto[len - 1] == '\n') {
        texto[len - 1] = '\0';
    }
    
    // Solicitar la palabra a buscar
    printf("Ingrese la palabra a buscar:\n");
    if (fgets(palabra, sizeof(palabra), stdin) == NULL) {
        fprintf(stderr, "Error al leer la palabra.\n");
        return 1;
    }
    len = strlen(palabra);
    if (len > 0 && palabra[len - 1] == '\n') {
        palabra[len - 1] = '\0';
    }
    
    // Buscar la ocurrencia de la palabra en la cadena
    int contador = 0;
    char *ptr = texto;
    
    printf("\nBuscando ocurrencias de la palabra \"%s\":\n", palabra);
    while ((ptr = strstr(ptr, palabra)) != NULL) {
        // Se muestra la posición en la que se encontró la palabra (se cuenta desde 0)
        printf("Encontrada en la posición: %ld\n", ptr - texto);
        contador++;
        // Avanzar el puntero para continuar la búsqueda (sin contar ocurrencias solapadas)
        ptr += strlen(palabra);
    }
    
    printf("Total de ocurrencias encontradas: %d\n", contador);
    
    return 0;
}