#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

void modify_int_declaration(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), input)) {
        // Buscar "int i = 0;" o cualquier otra asignación a 'i'
        char *pos = strstr(line, "int i = ");
        if (pos != NULL) {
            int value;
            // Extraemos el valor asignado a 'i' en caso de que sea una asignación
            if (sscanf(pos, "int i = %d;", &value) == 1) {
                // Escribir la primera línea con indentación: "int i;"
                fprintf(output, "\tint i;\n");
                // Escribir la segunda línea con indentación: "i = <valor>;"
                fprintf(output, "\ti = %d;\n", value);
                continue;
            }
        }
        
        // Escribir la línea sin cambios si no se encuentra el patrón
        fputs(line, output);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Error abriendo el archivo de entrada");
        return 1;
    }

    // Crear un archivo temporal para guardar los cambios
    FILE *temp = tmpfile();
    if (temp == NULL) {
        perror("Error creando archivo temporal");
        fclose(input);
        return 1;
    }

    // Modificar el archivo de entrada y guardar en el archivo temporal
    modify_int_declaration(input, temp);

    fclose(input);  // Cerrar el archivo original

    // Reabrir el archivo de entrada en modo escritura para sobrescribirlo
    input = fopen(argv[1], "w");
    if (input == NULL) {
        perror("Error abriendo el archivo de entrada para escritura");
        fclose(temp);
        return 1;
    }

    // Mover el puntero del archivo temporal al inicio
    fseek(temp, 0, SEEK_SET);

    // Copiar el contenido del archivo temporal al archivo original
    char ch;
    while ((ch = fgetc(temp)) != EOF) {
        fputc(ch, input);
    }

    fclose(temp);  // Cerrar el archivo temporal
    fclose(input); // Cerrar el archivo original

    printf("El archivo ha sido procesado correctamente.\n");
    return 0;
}

