#define _DEFAULT_SOURCE

#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "scan.h"

#define DIM_BUFFER 256

OutputFileScan * file_scan_1_svc(char **nomeFile, struct svc_req *rp){

    static OutputFileScan outputData;
    memset(&outputData, 0, sizeof(OutputFileScan));

    int fd = open(*nomeFile, O_RDONLY);

    if(fd < 0){
        //Launch rpc exception?
        return NULL;
    }

    int nread;
    char buf;

    while((nread = read(fd, &buf, sizeof(char))) > 0){
        if(nread < 0){
            //Launch rpc exception?
            return NULL;
        }

        if(buf == '\n') outputData.numeroLinee++;
        if(buf == ' ') outputData.numeroParole++;
        outputData.numeroCaratteri++;
    }

    close(fd);
    
    return (&outputData);

}

int * dir_scan_1_svc(InputDirScan *inputDirScan, struct svc_req *rp){
    static int risultato;
    risultato = 0;

    //Apro la cartella.
    DIR *myDir;
    struct dirent *currItem;

    myDir = opendir(inputDirScan->nomeDirettorio);

    if(myDir == NULL){
        //Launch rpc exception?
        risultato = -1;
        return (&risultato);
    }

    while((currItem = readdir(myDir)) != NULL){
        
        if(currItem->d_type == DT_REG){
            //Creo il path del file.
            char fileName[DIM_BUFFER];
            
            strcpy(fileName, inputDirScan->nomeDirettorio);
            strcat(fileName, "/");
            strcat(fileName, currItem->d_name);
            
            //Ricavo la dimensione del file per verificare la soglia
            struct stat statbuf;

            int statResult = stat(fileName, &statbuf);

            if(statResult < 0){
                //Launch rpc exception?
                risultato = -1;
                return (&risultato);
            }

            if(statbuf.st_size >= inputDirScan->sogliaDimensione) risultato++;
        }
    }

    closedir(myDir);

    return (&risultato);
}