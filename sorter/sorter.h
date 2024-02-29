#ifndef __SORTER_H__
#define __SORTER_H__

#define MAX_LINE_LENGTH 200
#define MAX_BUFFER_NUM  257


#define MAX_FOLDERS     100
#define MAX_FORMATS     100
#define MAX_RELATIONS   100
#define MAX_PATH        100

#define DEFAULT_PERIOD  300

#define TIMEOUT_DEFAULT_SEC 120

#define CONFIG_PATH "config"

#define SETPATH_COMMAND      "setpath"
#define SETFOLDER_COMMAND    "setfolder"
#define SETFORMAT_COMMAND    "setformat"
#define SETPERIODSEC_COMMAND "setperiod"

typedef struct {
    char* folder;
    char* value;
} relation_st_t;

typedef struct {
    char command[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
} parser_data_st_t;

extern char folders[MAX_FOLDERS][MAX_LINE_LENGTH];
extern int foldersCount;

extern char pathFolder[MAX_PATH][MAX_LINE_LENGTH];
extern int pathCount;

extern char formats[MAX_FORMATS][MAX_LINE_LENGTH];
extern int formatsCount;

extern relation_st_t relations[MAX_RELATIONS];
extern int relations_count;

#define MAX_TIMESTAMP 30

int start_application();

#endif /* __SORTER_H__*/