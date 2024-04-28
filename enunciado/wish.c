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

// Handle commands functions
int handle_builtin_commands(Vector items){
    if (strcmp(vector_get(&items, 0), "exit") == 0){
        if(items.size > 1){
            print_error();            
        } else {
            exit(0);
        }
        return 1;
    } else if (strcmp(vector_get(&items, 0), "cd") == 0) {
        if(chdir(vector_get(&items, 1)) != 0){
            // Test 1 and 2: Input to check bad cd. No arguments or 2 arguments are passed to cd.
            print_error();
        }
        return 1;
    } else if (strcmp(vector_get(&items, 0), "path") == 0){
        Vector new_path = vector_create();

        for (int i = 1; i < items.size; i++){
            char* aux = (char*) malloc((strlen(vector_get(&items, i)) + strlen("/") + 1) * sizeof(char));
            strcpy(aux, vector_get(&items, i));
            strcat(aux, "/");

            // Agregar el nuevo string a local_path
            vector_add(&new_path, aux);
        }

        PATH = new_path;
        return 1;
    }
    return 0;
}

int handle_external_commands(Vector items){
    char *command = vector_get(&items, 0);
    int pos = vector_get_index(&items, ">");
    if (pos == -1) pos = items.size;
    for (int i = 0; i < PATH.size; i++) {
        char *p = (char*) malloc((strlen(vector_get(&PATH, i)) + strlen(command) + 1) * sizeof(char));
        snprintf(p, strlen(vector_get(&PATH, i)) + strlen(command) + 1, "%s%s", vector_get(&PATH, i), command);
        
        if (access(p, X_OK) == 0) {
            char* argv[pos+1];
            for (int i = 0; i < pos; i++) argv[i] = vector_get(&items, i);
            argv[pos] = NULL;
            
            int rc = fork();
            if (rc == 0) {
                if (pos != items.size) {
                    close(STDOUT_FILENO);
                    open(
                        vector_get(&items, items.size - 1),
                        O_CREAT | O_WRONLY | O_TRUNC,
                        S_IRWXU
                    );
                }
                execv(p, argv);
            }
            return rc;
        }
    }

    print_error();
    return -1;   
}

char *search_script(char *filename){
    for (int i = 0; i < PATH.size; i++){
        char *dir = (char*) malloc((strlen(vector_get(&PATH, i)) + strlen(filename) + 1) * sizeof(char));
        snprintf(dir, strlen(vector_get(&PATH, i)) + strlen(filename) + 1, "%s%s", vector_get(&PATH, i), filename);
        if(access(dir, X_OK) == 0){
            return dir;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    char expression[256];
    Vector items;
    char *filename;
    int in_exec;

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

        in_exec = handle_builtin_commands(items);

        if(in_exec == 0){
            handle_external_commands(items);
        }        

        vector_destroy(&items);
    }
}

