#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "func.h"




int main(int argc, char** argv){
    AFD A;

    inicializarAFDVazio(&A);//passa o endereço de memória do scrtuct AFD A

    gerarAFD(&A, "afd.txt");
    //imprimir(A);
    printf("\n%d\n", A.numEstados);
    printf("%s", A.afd_Estado->prox->nomeEstado);
    printf("%s", A.afd_Estado->prox->prox->nomeEstado);

    printf("%d\n", A.numSimbolos);
    printf("%c\n", A.afd_Alfabeto->prox->Simbolo);
    printf("%c\n", A.afd_Alfabeto->prox->prox->Simbolo);



    printf("%d\n", A.numTransicao);

    return 0;
}