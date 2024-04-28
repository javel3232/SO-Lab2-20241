#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Global variables
const char *mypath[] = {
  "./",
  "/usr/bin/",
  "/bin/",
  NULL
};

char *local_path[] = {
  "./",
  "/usr/bin/",
  "/bin/",
  "tests/",
  NULL
};

char *builtin_commands[] = { 
    "exit",
    "cd",
    "path",
    NULL 
};

char *external_commands[] = {
    "ls",
    NULL 
};

// Utility functions
void print_error(){
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

int parse_input(char *expresion, char ***items, int *background){
    // Número máximo de tokens esperados
    int max_tokens = 64;
    // Delimitador para separar la cadena en tokens
    const char *delimitador = " \t\n";
    // Variable para almacenar cada token individual
    char *token;
    // Contador de tokens
    int num_tokens = 0;
    // Puntero a un array de punteros que contendrá los tokens
    char **tokens = (char **)malloc(max_tokens * sizeof(char *));

    if (tokens == NULL) {
        fprintf(stderr, "Error: Could not assign memory for token.\n");
        exit(EXIT_FAILURE);
    }

    // Obtener el primer token
    token = strtok(expresion, delimitador);

    // Mientras haya tokens y no se supere el número máximo de tokens
    while (token != NULL) {
        // Verificar si el token es "&" para indicar que el comando debe ejecutarse en segundo plano
        if (strcmp(token, "&") == 0) {
            *background = 1; // Indicar que el comando debe ejecutarse en segundo plano
            break; // Salir del bucle
        }

        // Almacenar el token actual en el array de tokens
        tokens[num_tokens++] = strdup(token);

        // Verificar si se superó el número máximo de tokens
        if (num_tokens >= max_tokens) {
            fprintf(stderr, "Warning: Max allowed tokens reached.\n");
            break; // Salir del bucle
        }

        // Obtener el siguiente token
        token = strtok(NULL, delimitador);
    }

    // Agregar NULL al final del array de tokens para indicar el final de la lista
    tokens[num_tokens] = NULL;

    // Asignar el array de tokens a la dirección de memoria apuntada por items
    *items = tokens;

    // Retornar el número de tokens encontrados
    return num_tokens;
}

int list_contains(char *str, char **list) {
    int i = 0;
    while (list[i] != NULL) {
        if (strcmp(list[i], str) == 0) {
            return 1;
        }
        i++;
    }
    return 0;
}

int count_elements(char **items) {
    int size = 0;
    // Iterar sobre el array hasta encontrar un puntero nulo
    while (items[size] != NULL) {
        size++;
    }
    return size;
}

// Handle commands functions
void handle_builtin_commands(char **items){
    if (strcmp(items[0], "exit") == 0) {
        // Test 5: Tries to exit with an argument. Should throw an error. (Exit con más de un argumento)
        if(count_elements(items) > 1){
            print_error();
        } else {
            exit(0);
        }
    } else if (strcmp(items[0], "cd") == 0) {
        if(chdir(items[1]) != 0){
            // Test 1 and 2: Input to check bad cd. No arguments or 2 arguments are passed to cd.
            print_error();
        }
    } else if (strcmp(items[0], "path") == 0) {
        fprintf(stderr, "Path still not created\n");
    }
}

void handle_external_commands(char **items){
    char *command = items[0];

    if (fork() == 0) {
        // Proceso hijo
        // Ejecutar el comando externo
        execvp(command, items);
        
        print_error();
    } else {
        // Proceso padre
        // Esperar a que el hijo termine
        wait(NULL);
    }
}

int main(int argc, char *argv[]){
    char expresion[256]; // Tamaño de la entrada
    char **items;
    int num, background;

    FILE *input_stream = stdin;

    if(argc == 2){
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            exit(1);
        }
    } else if(argc > 2) {
        exit(1);
    }

    while(1){
        // Verificar si estamos leyendo desde un archivo
        if (input_stream == stdin) {
            printf("wish> ");
        }

        // Leer la entrada
        fgets(expresion, sizeof(expresion), input_stream);

        // Verificar si se alcanzó el final del archivo (EOF)
        if (feof(input_stream)) {
            break;
        }

        num = parse_input(expresion, &items, &background);

        if(list_contains(items[0], builtin_commands)){
            handle_builtin_commands(items);
        } else if (list_contains(items[0], external_commands)){
            handle_external_commands(items);
        } else  {
            print_error();
        }
    }
}

