#ifndef PG2401_H20_10033_MAIN_H
#define PG2401_H20_10033_MAIN_H

#define MAIN 0
#define WORKER 1

typedef struct _READDATA {
    int bProgramFinished;
    int bThreadAtWork;
    int iCharsRead;
    int iSize;
    const char *szFileName;
    char *szText;
} READDATA;

void *workThreadInit(void *args);
void readFromFile(READDATA *readData, FILE *fpFile);
void writeToMemory(char *readText);
char *initializeFileName(int argc, char **argv);

#endif //PG2401_H20_10033_MAIN_H


