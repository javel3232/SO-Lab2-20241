#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

//  Creación de una estructura vector para facilitar el trabajo
#define VECTOR_CAPACITY 100

typedef struct Vector{
    char **data;
    int capacity;
    int size;
} Vector;

Vector vector_create() {
    Vector v;
    v.data = (char**) malloc (VECTOR_CAPACITY * sizeof(char*));
    v.capacity = VECTOR_CAPACITY;
    v.size = 0;
    return v;
}

void vector_add(Vector* v, char *str){
    if (v->capacity == v->size) {
        char** new_data = (char**) malloc( (v->capacity << 1) * sizeof(char*) );
        int n = v->size;
        for (int i = 0; i < n; i++) new_data[i] = v->data[i];
        free(v->data);
        v->data = new_data;
        v-> capacity <<= 1;
    }
    v->data[v->size++] = str;
}

void vector_remove_last(Vector* v){
    if (v->size > 0) v->size--;
}

char *vector_get(Vector* v, int index){
    if (index < 0 || index >= v->size) return NULL;
    return v->data[index];
}

int vector_get_size(Vector *v){
    return v->size;
}

int vector_get_index(Vector *v, char *val){
    int n = v->size;
    for(int i = 0; i < n; i++){
        if (strcmp(val, v-> data[i]) == 0) return i;
    }
    return -1;
}

void vector_destroy(Vector *v){
    free(v->data);
}

Vector PATH;

// Utility functions
void print_error(){
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

Vector parse_input(char *expresion){
    Vector items = vector_create();

    // Delimitador para separar la cadena en tokens
    const char *delimitador = " \t\n";
    // Variable para almacenar cada token individual
    char *token;

    // Obtener el primer token
    token = strtok(expresion, delimitador);

    while(token != NULL){
        vector_add(&items, token);

        // Obtener el siguiente token
        token = strtok(NULL, delimitador);
    }

    return items;
}

// Commands functions
void my_exit(Vector items){
    // Test 5: Tries to exit with an argument. Should throw an error. (Exit con más de un argumento)
    if(items.size > 1){
        print_error();
    } else {
        exit(0);
    }
}

void cd(Vector items){
    if(chdir(vector_get(&items, 1)) != 0){
        // Test 1 and 2: Input to check bad cd. No arguments or 2 arguments are passed to cd.
        print_error();
    }
}

void ls(Vector items){
    char *command = vector_get(&items, 0);

    if (fork() == 0) {
        // Proceso hijo
        // Ejecutar el comando externo
        execvp(command, items.data);
        
        print_error();
    } else {
        // Proceso padre
        // Esperar a que el hijo termine
        wait(NULL);
    }
}

void path(Vector items){
    Vector new_path = vector_create();
    for (int i = 1; i < items.size; i++){
        vector_add(&new_path, vector_get(&items, i));
    } 
    PATH = new_path;
}

void handle_scripts(Vector items){
    char *script = vector_get(&items, 0);
    for(int i = 0; i < PATH.size; i++) {
        char *dir = malloc(strlen(vector_get(&PATH, i)) + strlen(script) + 2); // Tamaño suficiente para almacenar la ruta completa
        if (dir == NULL) {
            perror("Error en malloc");
            exit(EXIT_FAILURE);
        }
        snprintf(dir, strlen(vector_get(&PATH, i)) + strlen(script) + 2, "%s/%s", vector_get(&PATH, i), script);

        // Verificar si el archivo existe
        if (access(dir, X_OK) == 0) {
            printf("El archivo '%s' existe en '%s'\n", script, vector_get(&PATH, i));
        }
        
        free(dir); // Liberar memoria asignada
    }

    print_error();
}

int main(int argc, char *argv[]){
    char expression[256];
    Vector items;

    vector_add(&PATH, "./");
    vector_add(&PATH, "/usr/bin/");
    vector_add(&PATH, "/bin/");

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
        fgets(expression, sizeof(expression), input_stream);

        // Verificar si se alcanzó el final del archivo (EOF)
        if (feof(input_stream)) {
            break;
        }

        items = parse_input(expression);

        if(strcmp(vector_get(&items, 0), "exit") == 0){
            my_exit(items);
        } else if (strcmp(vector_get(&items, 0), "cd") == 0) {
            cd(items);
        } else if (strcmp(vector_get(&items, 0), "ls") == 0) {
            ls(items);
        } else if (strcmp(vector_get(&items, 0), "path") == 0){
            
        } else {
            handle_scripts(items);
        }

        free(items.data);
    }
}

