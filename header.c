/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero                       <rmarrero@student.42barcelona.com>          +#+  +:+       +#+ #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define HEADER \
"\n" \
"\n" \
"\n" \
"\n" \
"\n" \
"\n" \
"\n" \
"\n" \
"\n" \
"\n" \
"\n\n"

void remove_comments(FILE *file) {
    char line[1024];
    long pos = ftell(file);      fseek(file, 0, SEEK_SET);

    FILE *temp_file = tmpfile();
    if (!temp_file) {
        perror("Error al crear archivo temporal");
        return;
    }

    int in_comment_block = 0;
    while (fgets(line, sizeof(line), file)) {
        char *ptr = line;
        while (*ptr) {
            if (in_comment_block) {
                if (ptr[0] == '*' && ptr[1] == '/') {
                    in_comment_block = 0;                      ptr++;
                }
            } else {
                if (ptr[0] == '/' && ptr[1] == '/') {
                    break;                  } else if (ptr[0] == '/' && ptr[1] == '*') {
                    in_comment_block = 1;                      ptr++;                  } else {
                    fputc(*ptr, temp_file);                  }
            }
            ptr++;
        }
    }

        fseek(file, pos, SEEK_SET);

        fseek(temp_file, 0, SEEK_SET);
    while (fgets(line, sizeof(line), temp_file)) {
        fputs(line, file);
    }

    fclose(temp_file);
}

void add_header_to_file(const char *filename) {
    FILE *file = fopen(filename, "r+");
    if (!file) {
        perror("Error al abrir el archivo");
        return;
    }

        remove_comments(file);

        fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(file_size + 1);
    if (!content) {
        perror("Error al asignar memoria");
        fclose(file);
        return;
    }
    fread(content, 1, file_size, file);
    content[file_size] = '\0';

        char name_before_at[256];
    const char *email = "rmarrero@student.42barcelona.com";     char *at_pos = strchr(email, '@');
    if (at_pos) {
        size_t name_length = at_pos - email;
        strncpy(name_before_at, email, name_length);
        name_before_at[name_length] = '\0';     } else {
        strcpy(name_before_at, "rmarrero");
    }

        char date_str[100];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(date_str, sizeof(date_str), "%Y/%m/%d %H:%M:%S", &tm);

        char header[512];
    snprintf(header, sizeof(header), HEADER, filename, name_before_at, date_str, date_str);

        freopen(filename, "w", file);      fputs(header, file);      fputs(content, file);  
        printf("%s: OK\n", filename);

    free(content);
    fclose(file);
}

void process_directory(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error al abrir el directorio");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {             const char *filename = entry->d_name;
            if (strstr(filename, ".c")) {                 add_header_to_file(filename);
            }
        }
    }
    closedir(dir);
}

int main() {
    process_directory(".");
    return 0;
}

0';

    // Obtener el nombre antes del @ del correo
    char name_before_at[256];
    const char *email = "rmarrero@student.42barcelona.com"; // El correo del autor
    char *at_pos = strchr(email, '@');
    if (at_pos) {
        size_t name_length = at_pos - email;
        strncpy(name_before_at, email, name_length);
        name_before_at[name_length] = '\0'; // Asegura que la cadena esté terminada
    } else {
        strcpy(name_before_at, "rmarrero");
    }

    // Obtener la fecha y hora actuales
    char date_str[100];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(date_str, sizeof(date_str), "%Y/%m/%d %H:%M:%S", &tm);

    // Prepara la cabecera con los datos actualizados
    char header[512];
    snprintf(header, sizeof(header), HEADER, filename, name_before_at, date_str, date_str);

    // Reescribir el archivo con la nueva cabecera
    freopen(filename, "w", file);  // Reabre el archivo en modo de escritura
    fputs(header, file);  // Escribe la nueva cabecera
    fputs(content, file);  // Escribe el contenido original sin comentarios

    // Imprime el nombre del archivo modificado seguido de "OK"
    printf("%s: OK\n", filename);

    free(content);
    fclose(file);
}

void process_directory(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error al abrir el directorio");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Solo archivos regulares
            const char *filename = entry->d_name;
            if (strstr(filename, ".c")) { // Solo archivos .c
                add_header_to_file(filename);
            }
        }
    }
    closedir(dir);
}

int main() {
    process_directory(".");
    return 0;
}

