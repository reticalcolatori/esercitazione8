#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "scan.h"

#define DIM 256

int main(int argc, char *argv[])
{
    CLIENT *client;
    char *server;

    char currOperation[DIM];

    if(argc < 2){
        perror("Usage: client nomenodoserver");
        exit(1);
    }

    server = argv[1];

    client = clnt_create(server, REMOTESCAN, REMOTESCANVERS, "udp");

    if(client == NULL){
        clnt_pcreateerror(server);
        exit(1);
    }

    //currOperation = (char*) malloc(DIM);
    printf("Inserisci il tipo di operazione che intendi effettuare, EOF per terminare\nF --> file_scan\nD --> dir_scan...\n");

    //parametri da richiedere all'utente (INPUT delle procedure RPC)
    char nomeFile[DIM];
    char *nomeFilePointer = nomeFile;
    //char nomeDir[DIM];
    //int soglia;
    InputDirScan inDirScan;

    //parametri che mi ritornano dalle procedure
    int *resDirScan;
    OutputFileScan *resFileScan;

    while(gets(currOperation)){

        if(currOperation[0] == 'F' || currOperation[0] == 'f'){
            //devo effettuare la scansione di un file
            //IN: nomedelfiledaanalizzare
            //OUT: struttura dati di tipo OutputFileScan (nCar, nParole, nLinee)

            printf("Inserisci il nome del file remoto da scansionare...\n");
            if(gets(nomeFile) == NULL){
                //EOF: esco
                exit(0);
            }

            printf("Nome file: %s\n", nomeFile);

            resFileScan = file_scan_1(&nomeFilePointer, client);

            if(resFileScan == NULL) {
                clnt_perror(client, server);
            } else {
                printf("Ricevuto dal server %s il risultato dell'analisi del file richiesto:\n\t1) Numero caratteri = %d\n\t2) Numero parole = %d\n\t3) Numero linee = %d\n", server, resFileScan->numeroCaratteri, resFileScan->numeroParole, resFileScan->numeroLinee);
            }
            
        }

        if(currOperation[0] == 'D' || currOperation[0] == 'd'){
            //devo effettuare la scansione di una directory remota
            //IN: nomecartella più soglia (struttura di tipo InputDirScan)
            //OUT: numero di file che soddisfa la specifica della soglia (DIMENSIONE) o < 0 se errore

            printf("Inserisci il nome della cartella da analizzare...\n");
            if(gets(inDirScan.nomeDirettorio) == NULL){
                //EOF
                exit(0);
            }

            printf("Nome direttorio: %s\n", inDirScan.nomeDirettorio);

            //TODO: inserisci la verifica ciclica del corretto inserimento
            printf("Inserisci la soglia minima per la dimensione dei file...\n");
            fscanf(stdin, "%d", &inDirScan.sogliaDimensione);

            printf("Soglia: %d\n", inDirScan.sogliaDimensione);

            resDirScan = dir_scan_1(&inDirScan, client);

            if(resDirScan == NULL) {
                clnt_perror(client, server);
            } else {
                printf("Ricevuto dal server %s il risultato dell'analisi della directory remota = %d\n", server, *resDirScan);
            }

        }

	//Mangio il fine linea.
	char tmpBuf[10];
	//gets(tmpBuf);

        printf("Inserisci il tipo di operazione che intendi effettuare, EOF per terminare\nF --> file_scan\nD --> dir_scan...\n");

    }

    clnt_destroy(client);
    exit(0);
}
