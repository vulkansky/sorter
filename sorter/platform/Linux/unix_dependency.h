#ifndef __UNIX_DEPENDENCY_H__
#define __UNIX_DEPENDENCY_H__

#include <dirent.h>

void check_relations(char* path, struct dirent* entry, char* format);
int process_files();
void create_directory_if_not_exists(const char* path, const char* value);
void wait_timeout(unsigned int sec);

#endif /* __UNIX_DEPENDENCY_H__ */