/*File XDR per la definzione del contratto*/

struct OutputFileScan{
  int numeroCaratteri;
  int numeroParole;
  int numeroLinee;
};

struct InputDirScan{
  string nomeDirettorio<50>;
  int sogliaDimensione;
};

program REMOTESCAN {
  version REMOTESCANVERS {
    OutputFileScan FILE_SCAN(string<50>) = 1;
    int DIR_SCAN(InputDirScan) = 2;
  } = 1;
} = 0x20000020;
