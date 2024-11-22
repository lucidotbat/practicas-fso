// getty.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_USERS 10
#define MAX_LENGTH 100

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

// Cargar usuarios desde el archivo passwd
void load_users() {
    FILE *file = fopen("passwd", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo passwd");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *username = strtok(line, ":");
        char *password = strtok(NULL, "\n");
        if (username && password) {
            strncpy(users[user_count].username, username, MAX_LENGTH - 1);
            users[user_count].username[MAX_LENGTH - 1] = '\0';
            strncpy(users[user_count].password, password, MAX_LENGTH - 1);
            users[user_count].password[MAX_LENGTH - 1] = '\0';
            user_count++;
            if (user_count >= MAX_USERS) {
                break;
            }
        }
    }
    fclose(file);
}

// Validar usuario y contraseña
int validate_user(const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            return 1; // Usuario válido
        }
    }
    return 0; // Usuario inválido
}

// Función para manejar el comando shutdown
void shutdown_system() {
    printf("Shutting down system...\n");
    kill(1, SIGKILL);  // Asumiendo que init tiene PID 1
}

int main() {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    load_users(); // Cargar usuarios desde el archivo passwd

    while (1) {
        printf("login: ");
        fflush(stdout);
        if (scanf("%s", username) != 1) {
            continue;
        }

        printf("password: ");
        fflush(stdout);
        if (scanf("%s", password) != 1) {
            continue;
        }

        if (strcmp(username, "shutdown") == 0 && strcmp(password, "") == 0) {
            // Permitir que 'shutdown' se ejecute sin contraseña si se desea
            shutdown_system();
            continue;
        }

        if (validate_user(username, password)) {
            printf("Login exitoso!\n");
            // Ejecutar el shell
            pid_t pid = fork();
            if (pid == 0) {
                // Proceso hijo: Ejecutar el shell
                execlp("./sh", "sh", NULL);
                perror("Error al ejecutar el shell");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                // Proceso padre: Esperar a que el shell termine
                wait(NULL);
                printf("Sesión terminada.\n");
            } else {
                perror("Error al hacer fork");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Credenciales inválidas, intenta de nuevo.\n");
        }
    }

    return 0;
}

