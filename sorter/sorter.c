#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif /* _WIN32 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "sorter.h"

#include "logger/logger.h"

#ifdef _WIN32
#include "platform/Windows/win_dependency.h"
#else /* UNIX */
#include "platform/Linux/unix_dependency.h"
#endif /* _WIN32 */

char folders[MAX_FOLDERS][MAX_LINE_LENGTH];
int foldersCount = 0;

char pathFolder[MAX_PATH][MAX_LINE_LENGTH];
int pathCount = 0;

char formats[MAX_FORMATS][MAX_LINE_LENGTH];
int formatsCount = 0;

relation_st_t relations[MAX_RELATIONS];
int relations_count = 0;

static FILE* config_file = NULL;

static int add_folder_to_array(const char* value)
{
        if (NULL == value) {
                LOG("Invalid input parameter: value = %p\n", value);

                return -1;
        }

        if (foldersCount < MAX_FOLDERS) {
                strcpy(folders[foldersCount], value);
                foldersCount++;
        } else {
                LOG("Maximum folders limit reached.\n");
        }

        return 0;
}

static int add_format_to_array(const char* value)
{
        if (NULL == value) {
                LOG("Invalid input parameter: value = %p\n", value);

                return -1;
        }

        if (formatsCount < MAX_FORMATS) {
                strcpy(formats[formatsCount], value);
                formatsCount++;
        } else {
                LOG("Maximum folders limit reached.\n");
        }

        return 0;
}

static int add_path_to_array(const char* value)
{
        if (NULL == value) {
                LOG("Invalid input parameter: value = %p\n", value);

                return -1;
        }

        if (pathCount < MAX_FORMATS) {
                strcpy(pathFolder[pathCount], value);
                pathCount++;
        } else {
                LOG("Maximum path limit reached.\n");
        }

        return 0;
}

static int init_config(const char* path)
{
        if (NULL == path) {
                LOG("Invalid input: path = %p\n", path);

                return -1;
        }

        config_file = fopen(path, "r");
        if (config_file == NULL) {
                LOG("Failed to open file %s\n", path);

                return -1;
        }

        return 0;
}

char* find_folder_by_value(const char* value)
{
        if (NULL == value) {
                LOG("Invalid input parameter: value = %p\n", value);

                return NULL;
        }

        for (int i = 0; i < foldersCount; i++) {
                if (strcmp(value, folders[i]) == 0) {
                        return folders[i];
                }
        }

    return NULL; /* Folder not found */
}

char* find_format_by_value(const char* value)
{
        if (NULL == value) {
                LOG("Invalid input parameter: value = %p\n", value);

                return NULL;
        }

        for (int i = 0; i < formatsCount; i++) {
                if (strcmp(value, formats[i]) == 0) {

                return formats[i];
                }
        }

    return NULL; /* Format not found */
}

int process_relations(char* command, char* value)
{
        relations[relations_count].folder = find_folder_by_value(command);
        relations[relations_count].value = find_format_by_value(value);

        if (relations[relations_count].folder == NULL || relations[relations_count].value == NULL) {
                LOG("Failed to create relations\n");

                relations[relations_count].folder = NULL;
                relations[relations_count].value = NULL;

                return -1;
        }

        relations_count++;

        return 0;
}

static int process_command(char* command, char* value)
{
        if (NULL == command || NULL == value) {
                LOG("Invalid input. command = %p, value = %p\n", command, value);

                return -1;
        }

        if (strcmp(command, SETPATH_COMMAND) == 0) {
                add_path_to_array(value);
        } else if (strcmp(command, SETFOLDER_COMMAND) == 0) {
                add_folder_to_array(value);
        } else if (strcmp(command, SETFORMAT_COMMAND) == 0) {
                add_format_to_array(value);
        } else if (0 != process_relations(command, value)) {
                LOG("Unexpected command\n");
        }

        return 0;
}

static int parse_config()
{
        int ret = 0;
        char line[MAX_LINE_LENGTH];
        char* command = NULL;
        char* value = NULL;
        char* delimiter = ":";
        char* separator = NULL;

        while (fgets(line, MAX_LINE_LENGTH, config_file) != NULL) {
                line[strcspn(line, "\n")] = 0;
                line[strcspn(line, "\r")] = 0;

                separator = strchr(line, delimiter[0]);

                if (separator != NULL) {
                        *separator = '\0';
                        command = line;
                        value = separator + 1;
                }

                if (command != NULL && value != NULL) {
                        ret = process_command(command, value);
                        if (0 != ret) {
                                LOG("Failed to parse command: ret = %d\n", ret);
                        }

                } else {
                        LOG("Invalid line format: %s\n", line);
                }
        }

        return 0;
}

int start_application()
{
        int ret = 0;

        ret = init_config(CONFIG_PATH);
        if (0 != ret) {
                LOG("Failed to initialization config. ret = %d\n", ret);

                return -1;
        }

        ret = parse_config();
        if (0 != ret) {
                LOG("Failed to parse config file\n");
        }

        fclose(config_file);

        for (int i = 0; i < pathCount; i++) {
                for (int j = 0; j < foldersCount; j++) {
                        create_directory_if_not_exists(pathFolder[i], folders[j]);
                }
        }

        while (1) {
        process_files();

        wait_timeout(4);

        }

        return 0;
}

#ifndef _WIN32
int main(int argc, char* argv[])
{
        (void)argc;
        (void)argv;

	start_application();

	return 0;
}
#endif /* _WIN32 */
