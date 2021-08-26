#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "func.h"



int main(int argc, char** argv){
   
    AFD A, B;
    int i = 0;
    char *palavra2 = "1110";
    char *pteste = "afd.txt";
    char *funcionalidade1, *funcionalidade2, *funcionalidade3, *funcionalidade4, *funcionalidade5, *funcionalidade6;
    funcionalidade1 = "--dot";
    funcionalidade2 = "--complemento";
    funcionalidade3 = "--intersecao";
    funcionalidade4 = "--uniao";
    funcionalidade5 = "--minimizacao";
    funcionalidade6 = "--reconhecer";


    //dot -Tpdf afd.dot > afd.pdf
    //Visialização: afdtool --dot afd.txt --output afd.dot
    if(argc > 1){
        if(comparaString(argv[1], funcionalidade1)){
            inicializarAFDVazio(&A);
            gerarAFD(&A, argv[2]);
            gerarDot(A, argv[4]);
        }
        //Complemento: afdtool --complemento afd.txt --output afd1-complemento.txt
        else if(comparaString(argv[1], funcionalidade2)){ 
            inicializarAFDVazio(&A);
            gerarAFD(&A, argv[2]);
            Complemento(A, argv[4]);

        }
        //Iter: afdtool --intersecao afd.txt afd.txt --output afd1-intersecao-afd2.txt

        else if(comparaString(argv[1], funcionalidade3)){
        /*  inicializarAFDVazio(&A);
            gerarAFD(&A, argv[2]);
            inicializarAFDVazio(&B);
            gerarAFD(&B, argv[3]);
            Intersecao(A, B, argv[5]);*/
            printf("\nChegou aqui1");
        }
        //União:  afdtool --uniao afd.txt afd1-complemento.txt --output afd1-uniao-afd2.txt
        else if(comparaString(argv[1], funcionalidade4)){
        /* inicializarAFDVazio(&A);
            gerarAFD(&A, argv[2]);
            inicializarAFDVazio(&B);
            gerarAFD(&B, argv[3]);
            Uniao(A,B);*/
            printf("\nChegou aqui2");
        }
        //Minimalização: fdtool --minimizacao afd1.txt --output afd1-minimizacao.txt

        else if(comparaString(argv[1], funcionalidade5)){
            inicializarAFDVazio(&A);
            gerarAFD(&A, argv[2]);
            minimaliza(A, argv[4]);
        }
        //Reconhecimento: afdtool --reconhecer afd.txt palavras.txt --output palavras-reconhecidas.txt
        else if(comparaString(argv[1], funcionalidade6)){
            inicializarAFDVazio(&A);
            gerarAFD(&A, argv[2]);
            reconhecerPalavra(A, argv[3], argv[5]);
        }
    }


    return 0;
    
}