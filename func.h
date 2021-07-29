
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
void gerarDot(AFD A){

    int i = 0;
    Estado *E;
    Transicao *T;

    /*abertura ou criação do arquivo*/
    FILE *pont_arq;
    pont_arq = fopen("afd.dot", "w");


    char c[20]          = {'"', '8', ',', '5'};
    //char aspasFim[4]    = {'"', ']', ';', '\n'};
    //char seta[4]        = {' ', '-', '>', ' '};
    char size[12]       = {'s', 'i', 'z', 'e', '=', '"', '8', ',', '5', '"', '\n'};
    //char label[11]      = {'[', 'l', 'a', 'b', 'e', 'l', ' ', '=', ' ', '"', '\n'};
    char aspas = {'"'};

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


