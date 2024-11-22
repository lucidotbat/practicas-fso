#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define STEPS 1000000000

// Función que calcula f(x) = sqrt(1 - x^2)
double f(double x) {
    return sqrt(1 - x * x);
}

int main() {
    struct timeval inicio, fin;
    
    // Variables para almacenar la integral, el tamaño de cada paso y el valor de x en cada iteración
    double integral, paso, x;
    
    // Obtener el tiempo de inicio antes de comenzar el cálculo
    gettimeofday(&inicio, NULL);
    
    // Calcular el tamaño de cada paso (delta x)
    paso = 1.0 / (double)STEPS;
    integral = 0.0;
    
    // Regla del trapecio: sumar las áreas de los trapecios
    for (int i = 0; i < STEPS; i++) {
        x = i * paso;  
        double x_ip1 = (i + 1) * paso;  
        integral += (f(x) + f(x_ip1)) * paso / 2.0;  // Área del trapecio
    }
    
    // El valor de la integral es pi/4, así que multiplicamos por 4
    double pi = 4 * integral;
    
    // Obtener el tiempo de fin después de terminar el cálculo
    gettimeofday(&fin, NULL);
    double tiempo_ejec = (fin.tv_sec - inicio.tv_sec) + (fin.tv_usec - inicio.tv_usec) / 1000000.0;
    
    printf("Valor calculado de PI: %.30f\n", pi);
    printf("Valor real de PI:      %.30f\n", M_PI);
    printf("Diferencia:            %.30f\n", fabs(pi - M_PI));
    printf("Tiempo de ejecución:   %.6f segundos\n", tiempo_ejec);
    
    return 0;
}
