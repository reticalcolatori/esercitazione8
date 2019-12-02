#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include "scan.h"

//Usage file_scan: host nomeFile
//Usage dir_scan: host nomeDir soglia

int main(int argc, char *argv[]){

    CLIENT *cl;

    //Arguments
    char *host;

    //file_scan arguments
    char *nomeFileScan;
    OutputFileScan *risFileScan;

    //dir_scan arguments
    InputDirScan inputDirScan;
    int *risDirScan;


    if(argc == 3){
        host = argv[1];
        nomeFileScan = argv[2];
    }else if(argc == 4){
        host = argv[1];
        inputDirScan.nomeDirettorio = argv[2];
        inputDirScan.sogliaDimensione = atoi(argv[3]);
    }else{
        fprintf(stderr, "Usage: %s host nomeFile\n", argv[0]);
        fprintf(stderr, "Usage: %s host nomeDir soglia\n", argv[0]);
		exit(1);
    }

    cl = clnt_create(host, REMOTESCAN, REMOTESCANVERS, "udp");

    if (cl == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

    if(argc == 3){
        risFileScan = file_scan_1(&nomeFileScan, cl);

        if(risFileScan == NULL){
            clnt_perror(cl, host);
            exit(1);
        }

        printf("Il file %s ha %d caratteri, %d linee e %d parole\n", nomeFileScan, risFileScan->numeroCaratteri, risFileScan->numeroLinee, risFileScan->numeroParole);
    }

    if(argc == 4){
        risDirScan == dir_scan_1(&inputDirScan, cl);

        if(risDirScan == NULL){
            clnt_perror(cl, host);
		    exit(1);
        }

        if(*risDirScan >= 0){
            printf("Il direttorio %s ha %d file che superano la soglia %d\n", inputDirScan.nomeDirettorio, *risDirScan, inputDirScan.sogliaDimensione);
        }else{
            printf("Il server ha restituito un errore: %d\n", *risDirScan);
        }
    }

    clnt_destroy(cl);

    return 0;
}
