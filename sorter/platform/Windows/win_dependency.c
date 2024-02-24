#define _CRT_SECURE_NO_WARNINGS

#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <windows.h>

#include "win_dependency.h"
#include "../../sorter.h"

void add_timestamp_to_filename(char* filename) {
    char timestamp[MAX_TIMESTAMP];

    time_t t = time(NULL);
    if (t == -1) {
        perror("Error getting time");
    }
    else {
        struct tm* tm_info = localtime(&t);
        if (tm_info == NULL) {
            perror("Error getting local time");
        }
        else {
            size_t result = strftime(timestamp, sizeof(timestamp), "_%Y%m%d%H%M%S", tm_info);
            if (result == 0) {
                printf("Error formatting time\n");
            }
            else {
                printf("timestamp: %s\n", timestamp);
            }
        }
    }

    char format[10] = "";
    char* dot = strrchr(filename, '.');
    if (dot && dot != filename) {
        strncpy(format, dot + 1, sizeof(format));
        *dot = '\0';
    }
    else {
        dot = filename + strlen(filename);
    }

    snprintf(dot, MAX_BUFFER_NUM - strlen(filename), "%s_%s.%s", dot ? "" : "_", timestamp, format);
}

void move_file_to_new_locate(const char* source, const char* destination)
{
    if (rename(source, destination) == 0) {
        printf("File '%s' moved to '%s'.\n", source, destination);
    }
    else {
        printf("Failed to move file %s to %s\n", source, destination);
    }
}

void check_relations(const char* path, const char* entry_name, const char* format) {
    char destinationPath[MAX_BUFFER_NUM + 20] = { 0 };
    char source[MAX_BUFFER_NUM + 20] = { 0 };

    for (int i = 0; i < relations_count; i++) {
        if (strcmp(format, relations[i].value) == 0) {
            snprintf(destinationPath, sizeof(destinationPath), "%s\\%s\\%s", path,
                relations[i].folder, entry_name);

            snprintf(source, sizeof(source), "%s\\%s", path, entry_name);

            struct stat buffer;
            if (stat(destinationPath, &buffer) == 0) {
                add_timestamp_to_filename(destinationPath);
            }

            move_file_to_new_locate(source, destinationPath);
            memset(destinationPath, '\0', MAX_BUFFER_NUM);
            memset(source, '\0', MAX_BUFFER_NUM);
        }
    }
}

int process_files() {
    struct _finddata_t file_data;
    intptr_t dir;
    printf("Pathcount: %d\n", pathCount);

    for (int i = 0; i < pathCount; i++) {
        char search_path[MAX_PATH];
        sprintf(search_path, "%s\\*", path_folder[i]);

        if ((dir = _findfirst(search_path, &file_data)) == -1) {
            perror("Unable to open directory");
            return 1;
        }

        do {
            if (file_data.attrib & _A_SUBDIR) {
                // Игнорируем поддиректории
                continue;
            }
            const char* dot = strrchr(file_data.name, '.');

            if (dot && dot != file_data.name) {
                printf("File: %s, Format: %s\n", file_data.name, dot + 1);
                check_relations(path_folder[i], file_data.name, dot + 1);
            }
            else {
                printf("File: %s, Format: unknown\n", file_data.name);
            }
        } while (_findnext(dir, &file_data) == 0);

        _findclose(dir);
    }

    return 0;
}

void create_directory_if_not_exists(const char* path, const char* value) {
    char fullpath[256];  // предполагаем, что путь не превысит 256 символов

    snprintf(fullpath, sizeof(fullpath), "%s\\%s", path, value);

    struct _stat st = { 0 };

    if (_stat(fullpath, &st) == -1) {
        if (_mkdir(fullpath) == 0) {
            printf("Directory '%s' created.\n", fullpath);
        }
        else {
            perror("Error creating directory");
        }
        }
    else {
        printf("Directory '%s' already exists.\n", fullpath);
    }
    }

void wait_timeout(unsigned int sec)
{
    DWORD milliseconds = 0;

    milliseconds = (DWORD)(sec * 1000);

    Sleep(milliseconds);
}
