#ifndef __WIN_DEPENDENCY_H__
#define __WIN_DEPENDENCY_H__

void check_relations(const char* path, const char* entry_name, const char* format);
int process_files();
void create_directory_if_not_exists(const char* path, const char* value);
void wait_timeout(unsigned int sec);

#endif /* __WIN_DEPENDENCY_H__ */