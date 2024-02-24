#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "sorter.h"
#include "unix_dependency.h"

void move_file_to_new_locate(const char *source, const char *destination)
{
        if (rename(source, destination) == 0) {
                printf("File '%s' moved to '%s'.\n", source, destination);
        } else {
                printf("Failed to move file %s to %s\n", source, destination);
        }
}

void add_timestamp_to_filename(char* filename)
{
        char timestamp[MAX_TIMESTAMP];
        char format[10] = "";

        time_t t = time(NULL);
        struct tm* tm_info = localtime(&t);
        strftime(timestamp, sizeof(timestamp), "_%Y%m%d%H%M%S", tm_info);

        char* dot = strrchr(filename, '.');
        if (dot && dot != filename) {
                strncpy(format, dot + 1, sizeof(format));
                *dot = '\0';
        } else {
                dot = filename + strlen(filename);
        }

        snprintf(dot, MAX_BUFFER_NUM - strlen(filename), "%s_%s.%s", dot ? "" : "_", timestamp, format);
}

void check_relations(char* path, struct dirent* entry, char* format)
{
        char destinationPath[MAX_BUFFER_NUM + 20] = { 0 };
        char source[MAX_BUFFER_NUM + 20] = { 0 };
        struct stat buffer;

        for (int i = 0; i < relations_count; i++) {
                if (strcmp(format, relations[i].value) == 0) {
                        snprintf(destinationPath, sizeof(destinationPath), "%s/%s/%s", path,
                        relations[i].folder, entry->d_name);

                        snprintf(source, sizeof(source), "%s/%s", path, entry->d_name);

                        if (stat(destinationPath, &buffer) == 0) {
                                add_timestamp_to_filename(destinationPath);
                        }

                        move_file_to_new_locate(source, destinationPath);

                        memset(destinationPath, '\0', MAX_BUFFER_NUM);
                        memset(source, '\0', MAX_BUFFER_NUM);
                }
        }
}

int process_files()
{
        DIR* dir;
        struct dirent* entry;

        for (int i = 0; i < pathCount; i++) {
                dir = opendir(pathFolder[i]);
                if (dir == NULL) {
                        printf("Unable to open directory\n");

                        return 1;
                }

                while ((entry = readdir(dir)) != NULL) {
                        if (entry->d_type == DT_REG) { 
                                char* dot = strrchr(entry->d_name, '.');

                                if (dot && dot != entry->d_name) {
                                        printf("File: %s, Format: %s\n", entry->d_name, dot + 1);

                                        check_relations(pathFolder[i], entry, dot + 1);
                                } else {
                                        printf("File: %s, Format: unknown\n", entry->d_name);
                                }
                        }
                }

                closedir(dir);
        }

        return 0;
}


void create_directory_if_not_exists(const char* path, const char* value)
{
        char fullpath[256] = {0};
        struct stat st = { 0 };

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, value);


        if (stat(fullpath, &st) == -1) {
                if (mkdir(fullpath, 0777) == 0) {
                        printf("Directory '%s' created.\n", fullpath);
                } else {
                        printf("Failed to create directory %s\n", fullpath);
                }
        } else {
                printf("Directory '%s' already exists.\n", fullpath);
        }
}

void wait_timeout(unsigned int sec)
{
        sleep(sec);
}
