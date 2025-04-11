#include <stdio.h>
#include <stdlib.h>
#include <math.h>


 /*
 * Se utiliza la fórmula iterativa para calcular cada coeficiente:
 *    c(0) = 1
 *    c(j) = c(j-1) * (i - j + 1) / j, para cada fila i y 1 <= j <= i
 */
void generarCoeficientes(int n, int *coef) {
    // Imprimir las filas desde 0 hasta n
    for (int i = 0; i <= n; i++) {
        // Imprimir espacios para centrar la fila
        for (int s = 0; s < (n - i); s++) {
            printf("   ");
        }
        
        long long c = 1;  // El primer coeficiente de cada fila es 1
        for (int j = 0; j <= i; j++) {
            // Se imprime el coeficiente con formato (ancho fijo)
            printf("%4lld   ", c);
            
            // Si estamos en la fila n, guardamos el coeficiente en el arreglo
            if (i == n) {
                coef[j] = c;
            }
            
            // Actualizamos el coeficiente para la siguiente posición
            c = c * (i - j) / (j + 1);
        }
        printf("\n\n");
    }
}

int main(){
    int n = 3; // Grado del polinomio (puedes cambiar este valor)
    int x = 5; // Valor de x para evaluar f(x)

    if(n < 0){
        fprintf(stderr, "n debe ser positivo.\n");
        return 1;
    }
    
    // Asignar memoria para n+1 coeficientes
    int *coef = (int *)malloc((n + 1) * sizeof(int));
    if (coef == NULL) {
        fprintf(stderr, "Error al asignar memoria.\n");
        return 1;
    }
    
    // Generar los coeficientes del polinomio (x+1)^n
    generarCoeficientes(n, coef);
    
    // Mostrar los coeficientes
    printf("Coeficientes del polinomio (x+1)^%d:\n", n);
    for (int i = 0; i <= n; i++){
        printf("%d ", coef[i]);
    }
    printf("\n");
    
    // Mostrar la representación del polinomio
    printf("Polinomio generado: ");
    for (int i = 0; i <= n; i++){
        printf("%d*x^%d", coef[i], i);
        if (i < n)
            printf(" + ");
    }
    printf("\n");
    
    // Evaluar f(x) = (x+1)^n mostrando cada paso
    int resultado = 0;
    printf("Cálculo de f(%d) = (x+1)^%d:\n", x, n);
    for (int i = 0; i <= n; i++){
        // Se utiliza la función pow de math.h para calcular x^i.
        // pow retorna un double, así que se convierte a int.
        int termino = coef[i] * ((int) pow(x, i));
        printf("  Paso %d: %d * %d^%d = %d\n", i+1, coef[i], x, i, termino);
        resultado += termino;
    }
    printf("f(%d) = %d\n", x, resultado);
    
    free(coef);
    return 0;
}
