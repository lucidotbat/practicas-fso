// init.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define NUM_GETTY 6
#define GETTY_PATH "./getty"  // Asegúrate de que getty está en el mismo directorio que init

pid_t getty_pids[NUM_GETTY];

// Función para iniciar un proceso getty en una ventana xterm
void start_getty(int index) {
    printf("Iniciando proceso getty %d\n", index);
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo: Ejecutar xterm con getty
        printf("Fork exitoso para proceso getty %d, llamando a execlp\n", index);
        execlp("xterm", "xterm", "-hold", "-e", GETTY_PATH, NULL);
        perror("Error al ejecutar xterm o getty");  // Se imprime si execlp falla
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Proceso padre: Guardar el PID del proceso getty
        printf("Proceso getty %d iniciado con PID %d\n", index, pid);
        getty_pids[index] = pid;
    } else {
        // Error en fork
        perror("Error al hacer fork");
        exit(EXIT_FAILURE);
    }
}

// Función para monitorear los procesos getty y reiniciarlos si terminan
void monitor_getty() {
    int status;
    while (1) {
        pid_t pid = wait(&status);
        if (pid == -1) {
            perror("Error en wait");
            exit(EXIT_FAILURE);
        } else {
            // Buscar qué getty terminó
            int found = 0;
            for (int i = 0; i < NUM_GETTY; i++) {
                if (getty_pids[i] == pid) {
                    printf("Proceso getty con PID %d terminó, reiniciando...\n", pid);
                    start_getty(i);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Proceso desconocido con PID %d terminó.\n", pid);
            }
        }
    }
}

int main() {
    // Verificar que el ejecutable getty existe y es ejecutable
    if (access(GETTY_PATH, X_OK) != 0) {
        perror("El ejecutable getty no existe o no tiene permisos de ejecución");
        exit(EXIT_FAILURE);
    }

    // Crear los 6 procesos getty inicialmente
    for (int i = 0; i < NUM_GETTY; i++) {
        start_getty(i);
    }

    // Monitorear los procesos getty
    monitor_getty();

    return 0;
}

