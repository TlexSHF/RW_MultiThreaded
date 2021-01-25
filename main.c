#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/main.h"

pthread_mutex_t mutexMain = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexWork = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t condMain =  PTHREAD_COND_INITIALIZER;
pthread_cond_t condWork =  PTHREAD_COND_INITIALIZER;


int main(int argc, char **argv) {
    pthread_t workThread;
    struct timespec *timeSpec; //TODO REMOVE
    int iSize = 10;
    char *szText;
    READDATA *readData;
    char *szFileName = initializeFileName(argc, argv);

    if(szFileName != NULL) {
        readData = malloc(sizeof(READDATA));
        if(readData == NULL) {
            printf("malloc failed\r\n");
        } else {
            memset(readData, 0, sizeof(READDATA));
            szText = malloc(sizeof(char) * iSize);
            if (szText == NULL) {
                printf("malloc failed\r\n");
            } else {
                memset(szText, 0, sizeof(char) * iSize);

                readData->szText = szText;
                readData->iSize = iSize;
                readData->bProgramFinished = 0;
                readData->iCharsRead = 0;
                readData->bThreadAtWork = WORKER;
                readData->szFileName = szFileName;

                pthread_create(&workThread, NULL, workThreadInit, readData);

                pthread_mutex_lock(&mutexMain);
                while(readData->bProgramFinished == 0) {

                    if(readData->bThreadAtWork == WORKER) {
                        pthread_cond_wait(&condMain, &mutexMain);
                    }

                    printf("%s", readData->szText);

                    readData->bThreadAtWork = WORKER;
                    pthread_mutex_lock(&mutexWork);

                    pthread_cond_signal(&condWork);
                    pthread_mutex_unlock(&mutexWork);

                }
                pthread_mutex_unlock(&mutexMain);
                pthread_join(workThread, NULL);

                printf("\r\nCharacters read: %i\r\n", readData->iCharsRead);

            }
            free(szText);
            free(readData);
        }
    }
    return 0;
}
void *workThreadInit(void *args) {
    FILE *fpFile;
    READDATA *readData = (READDATA *) args;

    fpFile = fopen(readData->szFileName, "r"); //TODO This will also be changed as parameter from user
    if (fpFile == NULL) {
        printf("error opening file\r\n");
    } else {
        printf("success opening file\r\n");

        pthread_mutex_lock(&mutexWork);
        while(readData->bProgramFinished == 0) {

            if(readData->bThreadAtWork == MAIN) {
                pthread_cond_wait(&condWork, &mutexWork);
            }

            readFromFile(readData, fpFile);

            readData->bThreadAtWork = MAIN;
            pthread_mutex_lock(&mutexMain);

            pthread_cond_signal(&condMain);
            pthread_mutex_unlock(&mutexMain);

        }
        pthread_mutex_unlock(&mutexWork);

        fclose(fpFile);
    }
}
void readFromFile(READDATA *readData,  FILE *fpFile) {
    int i = 0;
    char c;

    memset(readData->szText, 0, sizeof(char) * readData->iSize);

    while (i < readData->iSize) {
        c = fgetc(fpFile);

        if(c == EOF) {
            readData->bProgramFinished = 1;
            break;
        }
        readData->szText[i] = c;
        readData->iCharsRead++;
        i++;
    }
}
/* VALIDATES USER PARAMETERS */
char *initializeFileName(int argc, char **argv) {
    int i;
    char *szFileName = NULL;

    int j = 0;

    if(argc == 1) {
        printf("You need to specify what file you want to read. \r\n"
               "Write the full filename and file extension (if exists) as argument to execute.\r\n"
               "Like this: ./oppgave_6 filepath\r\n");
    } else if( argc > 1 && argv[1] != NULL) {
        szFileName = argv[1];

    } else {
        printf("An error occurred. Please try again\r\n");
    }

    return szFileName;
}