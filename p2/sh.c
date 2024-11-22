// sh.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void shutdown_system() {
    printf("Shutting down system...\n");
    kill(0, SIGKILL);  // Mata todos los procesos en el mismo grupo
}

void run_command(char *command, int background) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo: Ejecuta el comando
        char *args[] = { "sh", "-c", command, NULL };
        execvp(args[0], args);
        perror("Failed to execute command");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        if (!background) {
            // Espera si no es en segundo plano
            waitpid(pid, NULL, 0);
        } else {
            printf("Proceso %d ejecutándose en segundo plano.\n", pid);
        }
    } else {
        perror("Error al hacer fork");
    }
}

int main() {
    char command[256];

    while (1) {
        printf("sh > ");
        fflush(stdout);
        if (fgets(command, sizeof(command), stdin) == NULL) {
            continue;
        }
        command[strcspn(command, "\n")] = 0; // Eliminar el salto de línea

        if (strcmp(command, "exit") == 0) {
            break;  // Terminar shell
        } else if (strcmp(command, "shutdown") == 0) {
            shutdown_system();  // Apagar sistema
        } else if (command[strlen(command) - 1] == '&') {
            command[strlen(command) - 1] = 0;  // Ejecutar en segundo plano
            run_command(command, 1);
        } else {
            run_command(command, 0);  // Ejecutar en primer plano
        }
    }

    return 0;
}

