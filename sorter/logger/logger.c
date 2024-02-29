#include "logger.h"

void logToFile(const char *filename, const char *format, ...)
{
        va_list args;
        FILE *file;

        file = fopen(filename, "a");
        if (file == NULL) {
                perror("Error opening log file");

                return;
        }

        va_start(args, format);
        vfprintf(file, format, args);
        va_end(args);

        fclose(file);
}

