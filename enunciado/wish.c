#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Funcion para determinar el tamaño de la lista de items
int tamano_items(char **items) {
    int tamano = 0;
    // Iterar sobre el array hasta encontrar un puntero nulo
    while (items[tamano] != NULL) {
        tamano++;
    }
    return tamano;
}

/*
    Funcion para manejar cd
    Test 4: Input to run misc. commands. (cd before ls)
*/
void cd(char *args) {
    if(chdir(args) != 0){
        // Test 1 and 2: Input to check bad cd. No arguments or 2 arguments are passed to cd.
        fprintf(stderr, "An error has occurred\n");
    }
}

// Otras funciones de comandos internos...

/*
    Funcion para ejecutar comandos externos
    -ls (Test 3: ls with a bad directory name. Test 4: Run ls after cd)
*/
void ejecutar_externo(char *comando, char **args) {
    if (fork() == 0) {
        // Proceso hijo
        // Ejecutar el comando externo
        execvp(comando, args);
        
        exit(EXIT_FAILURE);
    } else {
        // Proceso padre
        // Esperar a que el hijo termine
        wait(NULL);
    }
}

int separaItems(char *expresion, char ***items, int *background) {
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
        fprintf(stderr, "Error: No se pudo asignar memoria para almacenar los tokens.\n");
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
            fprintf(stderr, "Advertencia: Se ha superado el número máximo de tokens permitidos.\n");
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

int main(int argc, char *argv[]) {
    const char *mypath[] = {
        "./",
        "/usr/bin/",
        "/bin/",
        NULL
    };

    char expresion[256]; // Tamaño de la entrada
    char **items;
    int num, background;

    FILE *input_stream = stdin;

    if (argc == 2) {
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            exit(1);
        }
    } else if (argc > 2) {
        exit(1);
    }

    while (1) {
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

        // Parse input
        num = separaItems(expresion, &items, &background);

        if(strcmp(items[0], "exit") == 0) {
            // Test 5: Tries to exit with an argument. Should throw an error. (Exit con más de un argumento)
            if(tamano_items(items) > 1){
                fprintf(stderr, "An error has occurred\n");
            } else {
                break;
            }
        } else if (strcmp(items[0], "cd") == 0) {
            cd(items[1]);
        } else if (strcmp(items[0], "ordenN") == 0) {
            // Lanzar el ejecutable asociado a la orden N
            // Codigo: suponiendo que es interna…
       
        } else {
            // Ejecutar comando externo
            ejecutar_externo(items[0], items);
        }
    }


    return 0;
}
