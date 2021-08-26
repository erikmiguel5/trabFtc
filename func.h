
/*criação das structs  do Estado Alfabeto Transição e Estados Finais*/
typedef struct Estado{
    char nomeEstado[25];
    struct Estado *prox;
}Estado;

typedef struct Alfabeto{
    char Simbolo;
    struct Alfabeto *prox;
}Alfabeto;

typedef struct Transicao{
    char estadoOrigem[25];
    char estadoDestino[25];
    char s;
    struct Transicao *prox;
}Transicao;


/*Struct do afd contento os atributos necessários para sua leitura*/
typedef struct AFD {
    int             numEstados;
    int             numSimbolos;
    int             numTransicao;
    int             numEFinais;
    Estado          *afd_Estado;
    Alfabeto        *afd_Alfabeto;
    Transicao       *afd_Transicao;
    Estado          *afd_eFinais;
    char            afd_eInicial[25];
}AFD;


/*inicializa estrutura do AFD*/
void inicializarAFDVazio(AFD* A){

    /*alocação de memória para os structs dentro da struct principal do AFD*/
    A->afd_Estado       = malloc (sizeof (Estado));
    A->afd_Alfabeto     = malloc (sizeof (Alfabeto));
    A->afd_Transicao    = malloc (sizeof (Transicao));
    A->afd_eFinais      = malloc (sizeof (Estado));
    /*inicializa os ponteiros de prox como NULL*/
    A->afd_Estado->prox         = NULL;
    A->afd_Alfabeto->prox       = NULL;
    A->afd_Transicao->prox      = NULL;
    A->afd_eFinais->prox        = NULL;
    /*inicia os contadores numéricos com valor 0*/
    A->numEstados   = 0;
    A->numSimbolos  = 0;
    A->numTransicao = 0;
    A->numEFinais   = 0;
}

/*função que recebe um string de estado e armazena ao final da lista de estados*/
void adicionarEstado(Estado *afd_Estado, char *nomeEstado){
    /*cria novo estado e armazena seu nome*/
    Estado *E = malloc (sizeof (Estado));
    E->prox = NULL;
    strcpy(E->nomeEstado, nomeEstado);

    /*insere o novo estado no final da lista de estados*/
    if(afd_Estado->prox == NULL){
        afd_Estado->prox = E;
    }
    else{
        Estado *aux = afd_Estado->prox;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = E;
    }       
}

/*adiciona um simbolo ao alfabeto*/
void adicionarAlfabeto(Alfabeto *afd_Alfabeto, char Simbolo){
    /*cria novo alfabeto e armazena seu nome*/
    Alfabeto *A = malloc (sizeof (Alfabeto));
    A->prox = NULL;
    A->Simbolo = Simbolo;

    /*insere o novo estado no final da lista de estados*/
    if(afd_Alfabeto->prox == NULL){
        afd_Alfabeto->prox = A;
    }
    else{
        Alfabeto *aux = afd_Alfabeto->prox;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = A;
    }   
}

/*adiciona o estado de origem, destido e simbolo a ser consumido em uma transição*/
void adicionarTransicao(Transicao *afd_Transicao, char *transicao){
    int i = 0;
    int j = 0;
    char c;
    char estado1[20];
    char estado2[20];

    /*laço para ler  o primeiro estado*/
    while(transicao[i] != ' '){
        estado1[i] = transicao[i];
        i++;
    }
    /*ler o simbolo*/
    i++;
    c = transicao[i];
    i++;
    i++;
    /*laço para ler o segundo estado*/
    while(i < strlen(transicao)){
        estado2[j] = transicao[i];
        j++;
        i++;
    }
     /*cria nova transição e armazena seus atributos*/
    Transicao *T = malloc (sizeof (Transicao));
    T->prox = NULL;
    strcpy(T->estadoOrigem, estado1);
    T->s = c;
    strcpy(T->estadoDestino, estado2);

    /*insere o nova transição no final da lista de transições*/
    if(afd_Transicao->prox == NULL){
        afd_Transicao->prox = T;
    }
    else{
        Transicao *aux = afd_Transicao->prox;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = T;
    }   
}

/*ler arquivo txt e gerar o automato*/
void gerarAFD(AFD* A, char *nomeArquivo){
    //ponteiro para o arquivo
    FILE *pont_arq;
    char texto_str[20];
    //abertura do arquivo
    pont_arq = fopen(nomeArquivo,"r");

    //testando se o arquivo foi aberto com sucesso
    int cont = 0;
    int aux;
    while(fgets(texto_str, 20, pont_arq) != NULL){
        aux = 0;
        //faz chamada da função para inserir os estados
        if(cont == 0){
            A->numEstados = atoi(texto_str);//transforma char em int
            while(cont < A->numEstados){
                fgets(texto_str, 20, pont_arq);
                adicionarEstado(A->afd_Estado, texto_str);
                cont++;
            }
        }
        //faz chamada da função para inserir os Símbolos do Alfabeto
        //numero de estados + o primeiro elemento
        else if(cont == A->numEstados + 1){
            A->numSimbolos = atoi(texto_str);
            while(aux < A->numSimbolos){
                fgets(texto_str, 20, pont_arq);
                adicionarAlfabeto(A->afd_Alfabeto, texto_str[0]);
                aux++;
                cont++;
            }
        }
        //faz chamada da função para inserir as Transições
        //numero de estados + numero de simbolos + 2 elementos
        else if(cont == (A->numEstados + A->numSimbolos + 2)){
            aux = 0;
            A->numTransicao = atoi(texto_str);
            while(aux < A->numTransicao){
                fgets(texto_str, 20, pont_arq);
                adicionarTransicao(A->afd_Transicao, texto_str);
                aux++;
                cont++;
            }
        }
        else{
            strcpy(A->afd_eInicial, texto_str);
            cont++;
            fgets(texto_str, 20, pont_arq);
            A->numEFinais = atoi(texto_str);
            aux = 0;
            cont++;
            while(aux < A->numEFinais){
                fgets(texto_str, 20, pont_arq);
                adicionarEstado(A->afd_eFinais, texto_str);
                aux++;
            }
        }
        cont++;
    }
    //fechando o arquivo
    fclose(pont_arq);
}

/*função para gerar o arquivo DOT*/
void gerarDot(AFD A, char nome[]){

    int i = 0;
    Estado *E;
    Transicao *T;

    /*abertura ou criação do arquivo*/
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");


    char c[20]          = {'"', '8', ',', '5'};
    char size[12]       = {'s', 'i', 'z', 'e', '=', '"', '8', ',', '5', '"', '\n'};
    char aspas          = {'"'};

    /*escrevendo no arquivo no formato DOT*/
    fputs("digraph finite_state_machine {\n", pont_arq);
    fputs("rankdir=LR;\n", pont_arq);
    fputs(size, pont_arq);

    /*estados finais*/
    fputs("node [shape = doublecircle];", pont_arq);
    E = A.afd_eFinais->prox;
    /*loop para percorrer cada estado*/
    while(E != NULL){
        fputs(" ", pont_arq);
        i = 0;
        /*loop para percorrer a string evitando o \n*/
        while(i < strlen(E->nomeEstado) && E->nomeEstado[i] != '\n'){
            putc(E->nomeEstado[i], pont_arq);
            i++;
        }
        E = E->prox;
    }
    fputs(";\n", pont_arq);
    fputs("node [shape = circle];\n", pont_arq);

    T = A.afd_Transicao->prox;
    /*loop para adicionar cada transição no formato DOT*/
    while(T != NULL){
        i = 0;
        /*loop para percorrer a string evitando o \n*/
        while(i < strlen(T->estadoOrigem) && T->estadoOrigem[i] != '\n'){
            putc(T->estadoOrigem[i], pont_arq);
            i++;
        }
        fputs(" -> ", pont_arq);
        i = 0;
        /*loop para percorrer a string evitando o \n*/
        while(i < strlen(T->estadoDestino) && T->estadoDestino[i] != '\n'){
            putc(T->estadoDestino[i], pont_arq);
            i++;
        }
        fputs(" [label = ", pont_arq);
        putc(aspas, pont_arq);
        putc(T->s, pont_arq);
        putc(aspas, pont_arq);
        fputs("];\n", pont_arq);
        T = T->prox;
    }
    putc('}', pont_arq);
    
    fclose(pont_arq);
    
}

/*função imprime para conferir os dados*/
void imprimir(AFD A){

    /*Ainda não foi criada a rotina para impressão, apenas printf's para teste*/
    printf("\n%d\n", A.numEstados);
    printf("%s", A.afd_Estado->prox->nomeEstado);
    printf("%s", A.afd_Estado->prox->prox->nomeEstado);

    printf("%d\n", A.numSimbolos);
    printf("%c\n", A.afd_Alfabeto->prox->Simbolo);
    printf("%c\n", A.afd_Alfabeto->prox->prox->Simbolo);
    printf("%d\n", A.numTransicao);

    printf("%s\n", A.afd_Transicao->prox->estadoOrigem);
    printf("%c\n", A.afd_Transicao->prox->s);
    printf("%s\n", A.afd_Transicao->prox->estadoDestino);

    printf("%s\n", A.afd_Transicao->prox->prox->estadoOrigem);
    printf("%c\n", A.afd_Transicao->prox->prox->s);
    printf("%s\n", A.afd_Transicao->prox->prox->estadoDestino);

    printf("%d\n", A.numTransicao);

    printf("%s\n", A.afd_Transicao->prox->prox->prox->prox->estadoOrigem);
    printf("%c\n", A.afd_Transicao->prox->prox->prox->prox->s);
    printf("%s\n", A.afd_Transicao->prox->prox->prox->prox->estadoDestino);

    printf("\n%s\n", A.afd_eInicial);
    printf("%d\n", A.numEFinais);
    printf("%s\n", A.afd_eFinais->prox->nomeEstado);
    
}




/*Funções Novas*/

void Complemento(AFD A, char nomeArquivo[])
{

    Estado *E;
    Transicao *T;
    Estado *EF;
    Alfabeto *AL;

    E = A.afd_Estado->prox;
    T = A.afd_Transicao->prox;
    EF = A.afd_eFinais->prox;
    AL = A.afd_Alfabeto->prox;

    int i =0; // Variavel para contabilizar o novo numero de estados Finais
    int estados =0;
    int EFs = 0;
    int cont =0;





    FILE *pont_arq; //(criando um txt para o complemento)

    pont_arq = fopen(nomeArquivo, "w");

    //Testando a abertura do arquivo
    if (pont_arq == NULL)
    {
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }
    //Gravando strings no arquivo


    fprintf(pont_arq,"%d\n",A.numEstados);
    cont = 0;
    //Retomando E para receber o parametro correto
    E = A.afd_Estado->prox;
    while(cont < A.numEstados)
    {
        fprintf(pont_arq,"%s",E->nomeEstado);
        E = E->prox;
        cont = cont+1;
    }

    fprintf(pont_arq,"%d\n",A.numSimbolos);
    cont = 0;
    while(cont < A.numSimbolos)
    {
        fprintf(pont_arq,"%c\n",AL->Simbolo);
        AL = AL->prox;
        cont = cont+1;
    }



    fprintf(pont_arq,"%d\n",A.numTransicao);
    cont =0;
    while(cont < A.numTransicao)
    {
        fprintf(pont_arq,"%s",T->estadoOrigem);
        fprintf(pont_arq," %c ",T->s);
        fprintf(pont_arq,"%s",T->estadoDestino);
        cont = cont+1;
        T = T->prox;
    }

    fprintf(pont_arq,"%s",A.afd_eInicial);

    fprintf(pont_arq,"%d\n",A.numEstados-A.numEFinais);


    int x;
    EFs =0;
    estados =0;
    E =A.afd_Estado->prox;
    EF = A.afd_eFinais->prox;

    while(A.numEstados>estados)
    {
        while(A.numEFinais> EFs)
        {
            //Verificando se o estado é diferente do EF, se for, agora vai se tornar EF
            x = comparaString(E->nomeEstado,EF->nomeEstado);
            if(x==0)
            {
                fprintf(pont_arq,"%s",E->nomeEstado);

            }

            EF = EF->prox;
            EFs = EFs +1;
        }

        EFs =0;
        EF = A.afd_eFinais->prox;
        E = E->prox;
        estados = estados+1;
    }



    //fechando o arquivo
    fclose(pont_arq);
}


//Função para gerar um txt
void GerarTxt(AFD A,char *nomeArquivo)
{

    Estado *E;
    Transicao *T;
    Estado *EF;
    Alfabeto *AL;

    E = A.afd_Estado->prox;
    T = A.afd_Transicao->prox;
    EF = A.afd_eFinais->prox;
    AL = A.afd_Alfabeto->prox;

    int cont;
    FILE *pont_arq; //(criando um txt para o complemento)

    pont_arq = fopen(nomeArquivo, "w");

    //Testando a abertura do arquivo
    if (pont_arq == NULL)
    {
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }
    //Gravando strings no arquivo


    fprintf(pont_arq,"%d\n",A.numEstados);
    cont = 0;
    //Laço que escreve no txt os estados
    while(cont < A.numEstados)
    {
        fprintf(pont_arq,"%s",E->nomeEstado);
        E = E->prox;
        cont = cont+1;
    }

    fprintf(pont_arq,"%d\n",A.numSimbolos);
    cont = 0;
    //Laço que escreve no txt os simbolos
    while(cont < A.numSimbolos)
    {
        fprintf(pont_arq,"%c\n",AL->Simbolo);
        AL = AL->prox;
        cont = cont+1;
    }



    fprintf(pont_arq,"%d\n",A.numTransicao);
    cont =0;
    //Laço que escreve no txt as transições
    while(cont < A.numTransicao)
    {
        fprintf(pont_arq,"%s",T->estadoOrigem);
        fprintf(pont_arq," %c ",T->s);
        fprintf(pont_arq,"%s",T->estadoDestino);
        cont = cont+1;
        T= T->prox;
    }

    fprintf(pont_arq,"%s",A.afd_eInicial);
    fprintf(pont_arq,"%d\n",A.numEFinais);
    cont = 0;

    //Laço que escreve no txt os estados finais
    while(cont < A.numEFinais)
    {
        fprintf(pont_arq,"%s",EF->nomeEstado);
        EF->prox = EF->prox;
        cont = cont+1;
    }


    //fechando o arquivo
    fclose(pont_arq);
}

void Produto(AFD A, AFD B)
{

    FILE *pont_arq;

    pont_arq = fopen("Produto.txt", "w");

    //Testando a abertura do arquivo
    if (pont_arq == NULL)
    {
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }

    Estado *E;
    Transicao *T;
    Estado *EF;
    Alfabeto *AL;

    Estado *E2;
    Transicao *T2;
    Estado *EF2;
    Alfabeto *AL2;

    E = A.afd_Estado->prox;
    T = A.afd_Transicao->prox;
    EF = A.afd_eFinais->prox;
    AL = A.afd_Alfabeto->prox;

    E2 = B.afd_Estado->prox;
    T2 = B.afd_Transicao->prox;
    EF2 = B.afd_eFinais->prox;
    AL2 = B.afd_Alfabeto->prox;

    int cont = 0;
    int cont2 =0;
    int p =0;

    //Gravando o numero de estados
    fprintf(pont_arq,"%d\n",A.numEstados * B.numEstados);
    char palavra[10];
    while(cont < A.numEstados)
    {
        while(cont2 < B.numEstados)
        {
            strcpy(palavra,E->nomeEstado);
            p=0;
            while(p<strlen(palavra) && palavra[p+1] != '\n')
            {
                fprintf(pont_arq,"%c",palavra[p]);
                p++;
                if(palavra[p+1] == '\n')
                {
                    fprintf(pont_arq,"%c",palavra[p]);
                }
            }
            //registrando os estados
            fprintf(pont_arq,",%s",E2->nomeEstado);
            cont2 = cont2+1;
            E2 = E2->prox;

        }
        cont2 = 0;
        E2 = B.afd_Estado->prox;
        E = E->prox;
        cont = cont+1;
    }
    //Gravando o numero de simbolos
    fprintf(pont_arq,"%d\n",A.numSimbolos);
    cont = 0;
    //Registrando os simbolos
    while (cont < A.numSimbolos)
    {
        fprintf(pont_arq,"%c\n",AL->Simbolo);
        AL = AL->prox;
        cont = cont+1;
    }
    //Gravando o numero de transições
    fprintf(pont_arq,"%d\n",A.numTransicao);
    cont =0;
    // Registrando as transições
    while(cont < A.numTransicao)
    {
        strcpy(palavra,T->estadoOrigem);
            p=0;
            while(p<strlen(palavra) && palavra[p+1] != '\n')
            {
                fprintf(pont_arq,"%c",palavra[p]);
                p++;
                if(palavra[p+1] == '\n')
                {
                    fprintf(pont_arq,"%c",palavra[p]);
                }
            }
            fprintf(pont_arq,",");

            strcpy(palavra,T2->estadoOrigem);
            p=0;
            while(p<strlen(palavra) && palavra[p+1] != '\n')
            {
                fprintf(pont_arq,"%c",palavra[p]);
                p++;
                if(palavra[p+1] == '\n')
                {
                    fprintf(pont_arq,"%c",palavra[p]);
                }
            }

        fprintf(pont_arq," %c ",T->s);
        strcpy(palavra,T->estadoDestino);
            p=0;
            while(p<strlen(palavra) && palavra[p+1] != '\n')
            {
                fprintf(pont_arq,"%c",palavra[p]);
                p++;
                if(palavra[p+1] == '\n')
                {
                    fprintf(pont_arq,"%c",palavra[p]);
                }
            }
        fprintf(pont_arq,",%s",T2->estadoDestino);
        cont = cont+1;
        T = T->prox;
        T2 = T2->prox;
    }
    strcpy(palavra,A.afd_eInicial);
    p=0;
    while(p<strlen(palavra) && palavra[p+1] != '\n')
    {
        fprintf(pont_arq,"%c",palavra[p]);
        p++;
        if(palavra[p+1] == '\n')
        {
            fprintf(pont_arq,"%c",palavra[p]);
        }
    }
    fprintf(pont_arq,",%s",B.afd_eInicial);

}
//Função que vai apenas ler os estados do produto
void LeituraEstadoProduto(AFD* A)
{
    //ponteiro para o arquivo
    FILE *pont_arq;
    char texto_str[40];
    //abertura do arquivo
    pont_arq = fopen("Produto.txt","r");

    //testando se o arquivo foi aberto com sucesso
    int cont = 0;
    int aux;
    while(fgets(texto_str, 40, pont_arq) != NULL)
    {
        aux = 0;
        //faz chamada da função para inserir os estados
        if(cont == 0)
        {
            A->numEstados = atoi(texto_str);//transforma char em int
            while(cont < A->numEstados)
            {
                fgets(texto_str, 40, pont_arq);
                adicionarEstado(A->afd_Estado, texto_str);
                cont++;
            }
        }



    }
}



void Uniao(AFD A, AFD B)
{   
    printf("\nentrou");
    //Produto(A,B);
    AFD C;
    inicializarAFDVazio(&C);
    LeituraEstadoProduto(&C);

    Estado *E;
    Estado *E2;
    Estado *EAF;
    Estado *EBF;

    E= A.afd_Estado->prox;
    E2= B.afd_Estado->prox;
    EAF=A.afd_eFinais->prox;
    EBF= B.afd_eFinais->prox;

    int vetA[16];
    int vetB[16];
    int estados;
    int EFs;
    int i;
    estados = 0;
    EFs =0;
    i = 0;
    int x;


    //Adicionando no vetor A as posições que estão os estados finais de A
    while(A.numEstados>estados)
    {
        while(A.numEFinais> EFs)
        {

            x = comparaString(E->nomeEstado,EAF->nomeEstado);

            if(x == 1)
            {

                vetA[i] = 1;
            }

            i++;
            EAF = EAF->prox;
            EFs = EFs +1;
        }

        EFs =0;
        EAF = A.afd_eFinais->prox;
        E = E->prox;
        estados = estados+1;
    }


    estados = 0;
    EFs =0;
    i = 0;

    //Adicionando no vetor B as posições que estão os estados finais de A
    while(B.numEstados>estados)
    {
        while(B.numEFinais> EFs)
        {
            x = comparaString(E2->nomeEstado,EBF->nomeEstado);
            if(x == 1)
            {
                vetB[i] = 1;
            }
            i++;
            EBF = EBF->prox;
            EFs = EFs +1;
        }

        EFs =0;
        EBF = B.afd_eFinais->prox;
        E2 = E2->prox;
        estados = estados+1;
    }


    Estado *EC;
    EC = C.afd_Estado->prox;
    int contador =0;
    int contA =0;
    int contB = 0;

    while(contA<A.numEstados)
    {
        while(contB<B.numEstados)
        {
            if((vetA[contA] == 1) || (vetB[contB] == 1))
            {
                adicionarEstado(C.afd_eFinais,EC->nomeEstado);
                contador = contador+1;
            }
            EC = EC->prox;
            contB++;
        }
        contB =0;
        contA++;

    }
    C.numEFinais = contador;


    FILE *F1;
    F1 = fopen("Produto.txt","r");
    FILE *F2;
    F2 = fopen("Uniao.txt","w");
    CopiaTXT(F1,F2);
    fclose(F1);
    fprintf(F2,"%d\n",C.numEFinais);

    int k = 0;
    Estado *ECF;
    ECF = C.afd_eFinais->prox;
    while(k<C.numEFinais)
    {
        fprintf(F2,"%s",ECF->nomeEstado);
        k++;
        ECF =ECF->prox;
    }
    fclose(F2);


}

void Intersecao(AFD A, AFD B, char nomeArquivo[])
{
    printf("\nentrou inter");
    //Produto(A,B);
    AFD C;
    inicializarAFDVazio(&C);
    LeituraEstadoProduto(&C);

    Estado *E;
    Estado *E2;
    Estado *EAF;
    Estado *EBF;

    E= A.afd_Estado->prox;
    E2= B.afd_Estado->prox;
    EAF=A.afd_eFinais->prox;
    EBF= B.afd_eFinais->prox;

    int vetA[16];
    int vetB[16];
    int estados;
    int EFs;
    int i;
    estados = 0;
    EFs =0;
    i = 0;
    int x;

    //Adicionando no vetor A as posições que estão os estados finais de A
    while(A.numEstados>estados)
    {
        while(A.numEFinais> EFs)
        {
            x = comparaString(E->nomeEstado,EAF->nomeEstado);
            if(x == 1)
            {
                vetA[i] = 1;
            }

            i++;
            EAF = EAF->prox;
            EFs = EFs +1;
        }

        EFs =0;
        EAF = A.afd_eFinais->prox;
        E = E->prox;
        estados = estados+1;
    }


    estados = 0;
    EFs =0;
    i = 0;

    //Adicionando no vetor B as posições que estão os estados finais de A
    while(B.numEstados>estados)
    {
        while(B.numEFinais> EFs)
        {
            x = comparaString(E2->nomeEstado,EBF->nomeEstado);
            if(x == 1)
            {
                vetB[i] = 1;
            }
            i++;
            EBF = EBF->prox;
            EFs = EFs +1;
        }

        EFs =0;
        EBF = B.afd_eFinais->prox;
        E2 = E2->prox;
        estados = estados+1;
    }


    Estado *EC;
    EC = C.afd_Estado->prox;
    int contador =0;
    int contA =0;
    int contB = 0;

    while(contA<A.numEstados)
    {
        while(contB<B.numEstados)
        {
            if((vetA[contA] == 1) && (vetB[contB] == 1))
            {
                adicionarEstado(C.afd_eFinais,EC->nomeEstado);
                contador = contador+1;
            }
            EC = EC->prox;
            contB++;
        }
        contB =0;
        contA++;

    }
    C.numEFinais = contador;



    FILE *F1;
    F1 = fopen("Produto.txt","r");
    FILE *F2;
    F2 = fopen(nomeArquivo,"w");
    CopiaTXT(F1,F2);
    fclose(F1);
    fprintf(F2,"%d\n",C.numEFinais);

    int k = 0;
    Estado *ECF;
    ECF = C.afd_eFinais->prox;
    while(k<C.numEFinais)
    {
        fprintf(F2,"%s",ECF->nomeEstado);
        k++;
        ECF =ECF->prox;
    }
    fclose(F2);
}

void EscreveTXT(char palavra[], FILE *F)
{
    int i=0;
    while(i < strlen(palavra) && palavra[i+1] != '\n')
    {
        fputs(palavra[i],F);
        i++;
        if(palavra[i+1] == '\n')
        {
            fputs(palavra[i],F);
        }
    }


}

void CopiaTXT(FILE *F1, FILE *F2)
{
    char leitor[50];
    while(fgets(leitor,50,F1) != NULL)
    {
        fputs(leitor,F2);
    }
}



/*Compara duas string, retorna 1 se foram iguais, e 0 caso contrário*/
int comparaString(char string1[], char string2[]){

    int i = 0;
    while(i < strlen(string1) && string1[i+1] != '\n'){ 
        if(string1[i] != string2[i]){
             return 0;
        }
        i++;
    }
    i = 0;
    while(i < strlen(string2) && string2[i+1] != '\n'){ 
        if(string1[i] != string2[i]){
             return 0;
        }
        i++;
    }

    return 1;
}



int realizarTransicao(AFD A, char *estadoAtual, char s){
    Transicao *aux = A.afd_Transicao->prox;
    int tamanhoEstado = strlen(estadoAtual);
    while(aux != NULL){
        if(comparaString(aux->estadoOrigem, estadoAtual) && aux->s == s){
            strcpy(estadoAtual, aux->estadoDestino);
            return 1;
        }
        aux = aux->prox;
    }
    
    return 0;
}


/*Reconhecer palavra*/
int verificaPalavra(AFD A, char *palavra){
    Estado *aux = A.afd_eFinais->prox;
    int i = 0;
    int tamanhoPalavra = strlen(palavra);
    char *estadoAtual = A.afd_eInicial;

    while(i < tamanhoPalavra){
        if(realizarTransicao(A, estadoAtual, palavra[i]) == 0) return 0;
        i++;
    }
    while(aux != NULL){
        if(comparaString(estadoAtual, aux->nomeEstado)) return 1;
        aux = aux->prox;
    }
    return 0;
}

void reconhecerPalavra(AFD A, char entradatxt[], char saidatxt[]){

    //ponteiro para o arquivo
    FILE *arqEntrada;
    FILE *arqSaida;
    char *texto_str;
    //abertura do arquivo
    arqEntrada  = fopen(entradatxt,"r");
    arqSaida    = fopen(saidatxt,"w");

    while(fscanf(arqEntrada,"%s", texto_str) != EOF){
        //char c = verificaPalavra(A, texto_str);
        //printf("\n aqui : %s", texto_str);
        fprintf(arqSaida, "%d\n", verificaPalavra(A, texto_str));
    }

   //fprintf(fp, "%s %s %s %d", "We", "are", "in", 2012);
   
   fclose(arqEntrada);
   fclose(arqSaida);

}

    //eliminar estados inalcançaveis
    void eliminarEstadosInal(AFD A){
        Estado *aux = A.afd_Estado->prox;
        Transicao *T;
        char *aux2;
        int flag;
        while(aux->prox != NULL){
            flag = 0;
            T = A.afd_Transicao->prox;
            if(!comparaString(aux->nomeEstado, A.afd_eInicial)){
                while(T->prox != NULL){
                    if(comparaString(aux->nomeEstado, T->estadoDestino)) flag = 1;
                    T = T->prox;
                }
                if(!flag){
                    aux2 = aux->nomeEstado;
                    aux = A.afd_Estado->prox;
                    while(comparaString(aux->prox->nomeEstado, aux2)) aux = aux->prox;
                    aux->prox = aux->prox->prox;
                }
            }
            aux = aux->prox;
        }
    }


void minimaliza(AFD A, char nomeArquivo[]){

    int n;
    Estado *finais;
    Estado *naoFinais;
    eliminarEstadosInal(A);
}
