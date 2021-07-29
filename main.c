#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "func.h"




int main(int argc, char** argv){
    AFD A;

    inicializarAFDVazio(&A);//passa o endereço de memória do scrtuct AFD A

    gerarAFD(&A, "afd.txt");
    //imprimir(A);
    gerarDot(A);



    return 0;
}