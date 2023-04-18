/*
    Ficheiro: project1.c
    Autora: Francisca Vicente de Almeida (ist1105901)
    Descricao: Gestor de carreiras e paragens de autocarros (projeto 1 de IAED).
*/

#include "project1.h"

/*   Variaveis Globais   */

int N_carreiras = 0, N_paragens = 0;

Carreira stored_carreiras[MAX_CARREIRAS];
Paragem stored_paragens[MAX_PARAGENS];

int main() {
    processar_comandos();
    return 0;
}

/* FUNCAO A SER MODIFICADA - TESTE PRATICO */
/*  
    Funcao  que recebe os comandos introduzidos pelo utilizador e permite que
    estes funcionem,  sendo  que, enquanto 'running' for verdadeiro o programa
    continua a correr até ser executado o comando 'q'.
*/                          
void processar_comandos() {
    int running = TRUE;

    while (running) {
        char command = getchar();
        /*
            Se o comando for diferente de 'q', 'c', 'p', 'l' ou 'i' ignora-o e
            passa para o proximo.
        */
        switch (command) {
            case 'q':
                running = FALSE;
                /*
                    Se o comando for 'q' sai do ciclo e termina o programa.
                */
                break;

            /*
                Se o comando for 'c', 'p', 'l' ou 'i' chama a funcao que trata
                desse comando.
            */
            case 'c':
                processar_carreiras();
                break;
            case 'p':
                processar_paragens();
                break;
            case 'l':
                processar_ligacao();
                break;
            case 'i':
                processar_interligacao();
                break;
                /* Novo comando - teste pratico */
            case 'f':
                processar_fim();
                break;
        }
    }
}

/*
    Esta  funcao  trata dos argumentos forncedido pelo utilizador, separando-os
    por palavras, ja tendo em consideracao os casos dos nomes de carreiras e
    paragens que podem ser fornecidos entre aspas e guardando-os numa matriz
    de caracteres.
*/
void leLinha(char list_of_words[][MAX_INPUT], int *argumentos) {
    int i = 0, c, aspas = FALSE, comecar = FALSE;
    /* Define o valor inicial para o número de argumentos */
    *argumentos = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        /* Ignora o primeiro espaco entre o comando e o primeiro argumento */
        if(!comecar) {
            comecar = TRUE;
            continue;
        }
        
        if ((isspace(c) || c == '\t') && !aspas) {
            /* 
                Ignora espaços em branco extras entre argumentos do
                comando (quando nao e um nome entre aspas)
            */
            if (i == 0)
                continue;

            /* Adiciona no argumento atual o caracter terminal */
            list_of_words[*argumentos][i] = '\0';
            (*argumentos)++;
            i = 0;
        }
        else {
            /* Verifica se estamos dentro de um par de aspas */
            if (c == '"') {
                aspas = !aspas;
                continue;
            }
            /* Adiciona o caractere atual à palavra atual */
            list_of_words[*argumentos][i++] = c;
        }
    }
    /* Adiciona a última palavra o caracter terminal */
    if (i > 0) {
        list_of_words[*argumentos][i] = '\0';
        (*argumentos)++;
    }
}


/*
    Esta  funcao  verifica  se  um  determinado argumento pode ser o
    equivalente ao parametro opcional da palavra "inverso".
*/
int eh_inverso(char str[]) {
    char inverso[8];
    int i, len_str = strlen(str), len_inv;

    strcpy(inverso, "inverso");

    len_inv = strlen(inverso);

    /* Se tiver menos de tres caracteres retorna falso */
    if (len_str < 3)
        return FALSE;

    for (i = 0; len_inv < len_str ? i < len_inv : i < len_str; i++) {
        /* Se os caracteres forem diferentes retorna falso */
        if (str[i] != inverso[i])
            return FALSE;
    };

    /* 
        No resto dos casos retorna verdadeiro, ou seja,
        inv, inve, inver, invers, inverso 
    */
    return TRUE;
}

/* Insere um elemento no comeco de uma linked list */
Node *inserir_no_inicio(Node *head, int new_id) {
    Node *novo = malloc(sizeof(Node));
    novo->id = new_id;
    /* 
        O primeiro elemento passa a ter um elemento anterior (o novo primeiro)
        e o novo primeiro passa a ter um elemento seguinte (o antigo primeiro)
    */
    head->prev = novo;
    novo->next = head;
    novo->prev = NULL;
    head = novo;
    return head;
}

/* Insere um elemento no fim de uma linked list */
Node *inserir_no_final(Node *tail, int new_id) {
    Node *novo = malloc(sizeof(Node));
    /*
        O ultimo elemento passa a ter um elemento seguinte (o novo ultimo)
        e o novo ultimo passa a ter um elemento anterior (o antigo ultimo)
    */
    novo->id = new_id;
    tail->next = novo;
    novo->next = NULL;
    novo->prev = tail;
    tail = novo;
    return tail;
}

/* Faz print dos elementos da linked list com o formato A, B, C\n */
void print_lista_normal(Node * lista) {
    Node *aux = lista;
    Paragem paragem;
    /* 
        Enquanto o id da variavel aux existir, ele continua a procura das 
        paragens para fazer print do nome das mesmas 
    */
    while (aux->id) {
        paragem = obter_paragem_por_id(aux->id);
        if(!(aux->next)) { /* Se for a ultima da linked list */
            printf("%s\n", paragem.nome);
            return;
        }
        printf("%s, ", paragem.nome); /* Se nao for a ultima */
        aux = aux->next; /* Vai buscar o proximo */
    }
}

/* 
    Mesmo objetivo e logica da funcao print_lista_normal no entanto com
    ordem decrescente.
*/
void print_lista_inverso(Node * lista) {
    Node *aux = lista;
    Paragem paragem;
    /*
        Enquanto o id da variavel aux existir, ele continua a procura das 
        paragens (no caso, da anterior) para fazer print do nome das mesmas
    */
    while (aux->id) {
        paragem = obter_paragem_por_id(aux->id);
        if(!(aux->prev)) {
            printf("%s\n", paragem.nome);
            return;
        }
        printf("%s, ", paragem.nome);
        aux = aux->prev;
    }
}

/* Retorna o valor de numero de paragens de uma carreira */
int obter_numero_paragens(int carreira) {
    return stored_carreiras[carreira - 1].numero_paragens;
}

/* Retorna o valor de numero de carreiras de uma paragem */
int obter_numero_carreiras(int paragem) {
    return stored_paragens[paragem - 1].numero_carreiras;
}

/* 
    Se uma paragem com o nome indicado existir retorna o indice da paragem (+1)
    e caso nao exista retorna 0 
*/
int paragem_existe(char nome[]) {
    int i;
    /* 
        Vai a procura dentro de todas as paragens a paragem de indice i com o
        nome da paragem fornecido nos argumentos da funcao
    */
    for (i = 0; i < N_paragens; i++) {
        if (strcmp(stored_paragens[i].nome, nome) == 0) {
            return i + 1;
        }
    }
    return FALSE;
}

/* Retorna a paragem de indice id-1 */
Paragem obter_paragem_por_id(int id) {
    return stored_paragens[id - 1];
}


/* FUNCAO A SER MODIFICADA - TESTE PRATICO */

/* Funcao que cria uma paragem dentro da lista stored_paragens */
int criar_paragem(char nome[], Reais latitude, Reais longitude) {
    Paragem nova_paragem;
    /* Se a paragem existir retorna um erro */
    if(paragem_existe(nome)) {
        printf("%s: stop already exists.\n", nome);
        return FALSE;
    }
    /* 
        Teste pratico - analisa a altitude e latitude se estao nos intervalos 
        pretendidos
    */
    if(latitude < -90 || latitude > 90) {
        printf("invalid location.\n");
        return FALSE;
    }

    if(longitude < -180 || longitude > 180) {
        printf("invalid location.\n");
        return FALSE;
    }

    /* 
        Define os valores default na paragem nova_paragem e posteriormente
        acrescenta na lista das paragens 
    */
    strcpy(nova_paragem.nome, nome);
    nova_paragem.latitude = latitude;
    nova_paragem.longitude = longitude;
    nova_paragem.numero_carreiras = 0;
    
    stored_paragens[N_paragens++] = nova_paragem;

    return TRUE;
}

/*
    Funcao que verifica se uma carreira existe, se existir retorna o indice
    do elemento +1 ou 0 se nao existir na lista. 
*/
int carreira_existe(char nome[]) {
    /*
        Vai a procura dentro de todas as carreiras a carreira de indice i com
        o nome da carreira fornecido nos argumentos da funcao
    */
    int i;
    for (i = 0; i < N_carreiras; i++) {
        if (strcmp(stored_carreiras[i].nome, nome) == 0)
            return i + 1;
    }
    return FALSE;
}

/* Retorna a carreira de indice id-1 */
Carreira obter_carreira_por_id(int id) {
    return stored_carreiras[id - 1];
}

/* Funcao que cria uma carreira dentro da lista stored_carreiras */
void criar_carreira(char nome[]) {
    Carreira nova_carreira;
    /*
        Define os valores default na carreira nova_carreira e posteriormente
        acrescenta na lista das carreiras.
    */
    strcpy(nova_carreira.nome, nome);
    nova_carreira.numero_paragens = 0;
    nova_carreira.custo = 0;
    nova_carreira.duration = 0;

    stored_carreiras[N_carreiras++] = nova_carreira;
}

/* 
    Funcao que permite verificar se os argumentos fornecidos para a funcao
    criar ligacao sao validos, se forem retorna 1, caso contrario retorna 0
*/
int verificar_lig(char carreira_nome[], char nome_paragem1[],
                  char nome_paragem2[], Reais custo,
                  Reais duracao) {
    int numero_paragem1, numero_paragem2, numero_carreira;
    Carreira carreira;

    numero_carreira = carreira_existe(carreira_nome);

    /* Se a carreira nao foi encontrada retorna erro */
    if(!numero_carreira) {
        printf("%s: no such line.\n", carreira_nome);
        return FALSE;
    }

    numero_paragem1 = paragem_existe(nome_paragem1);
    numero_paragem2 = paragem_existe(nome_paragem2);

    /* Se uma das paragens nao foi encontrada retorna erro */
    if (!numero_paragem1 || !numero_paragem2) {
        printf("%s: no such stop.\n", !numero_paragem1 ?
        nome_paragem1 : nome_paragem2);
        return FALSE;
    }

    carreira = obter_carreira_por_id(numero_carreira);

    /* 
        No caso das paragens da ligação, origem ou destino,
        não correspoderem a um dos extremos da carreira 
    */
   
    if(obter_numero_paragens(numero_carreira) > 0 && 
    !(carreira.paragens_normal->id == numero_paragem2 ||
    carreira.paragens_inverso->id == numero_paragem1)) {
        printf("link cannot be associated with bus line.\n");
        return FALSE;
    }
    
    if(custo < 0 || duracao < 0) {
        printf("negative cost or duration.\n");
        return FALSE;
    }

    return TRUE;

}

/* 
    Funcao que define as paragens de uma carreira quando a carreira 
    nao tem paragens, sendo que a paragem 1 eh a origem e a paragem
    2 eh o destino
*/
void definir_paragens_carreira(int numero_carreira, int numero_paragem1,
                               int numero_paragem2) {
        Paragem paragem1 = obter_paragem_por_id(numero_paragem1);
        Paragem paragem2 = obter_paragem_por_id(numero_paragem2);

        int carreiras_1 = paragem1.numero_carreiras;
        int carreiras_2 = paragem2.numero_carreiras;

        Node *head = malloc(sizeof(Node)), *tail = malloc(sizeof(Node));
        /*
            Define os valores default para as paragens da carreira
            e posteriormente acrescenta na lista das paragens
        */
        head->id = numero_paragem1;
        tail->id = numero_paragem2;
        head->next = tail;
        tail->prev = head;
        head->prev = NULL;
        tail->next = NULL;
        /*
            Define as paragens da carreira como sendo a
            paragem 1 (origem) e a paragem 2 (destino)
        */
        stored_carreiras[numero_carreira-1].paragens_normal = head;
        stored_carreiras[numero_carreira-1].paragens_inverso = tail;
        /*
            Acrescenta a carreira na lista de carreiras da paragem
            e incrementa o numero de carreiras da paragem
        */
        stored_paragens[numero_paragem1-1].carreiras[carreiras_1] = 
        numero_carreira;
        stored_paragens[numero_paragem2-1].carreiras[carreiras_2] = 
        numero_carreira;
        stored_paragens[numero_paragem1-1].numero_carreiras++;
        stored_paragens[numero_paragem2-1].numero_carreiras++;
        /*
            Define o numero de paragens da carreira como 2, uma vez que
            a carreira so tem 2 paragens (origem e destino)
        */
        stored_carreiras[numero_carreira-1].numero_paragens=2;
}

/*
    Funcao que adiciona uma paragem a uma carreira, sendo que a paragem
    pode ser adicionada no inicio ou no fim da carreira
*/
void adicionar_paragens_carreira(int carreira, int paragem, int comeco) {
    int n_carreiras = obter_numero_carreiras(paragem); 
    /* 
        Se a paragem nao passa pela carreira, acrescenta a carreira
        na lista de carreiras da paragem e incrementa o numero de carreiras
        da paragem
    */
    if(!carreira_em_paragem(carreira, paragem)) {
        stored_paragens[paragem - 1].carreiras[n_carreiras] = carreira;
        stored_paragens[paragem - 1].numero_carreiras++;
    }
    /*
        Se a paragem for adicionada no inicio da carreira, insere a paragem
        no inicio da lista de paragens da carreira, caso contrario insere
        a paragem no fim da lista de paragens da carreira
    */
    if(comeco) {
        stored_carreiras[carreira - 1].paragens_normal = 
        inserir_no_inicio(stored_carreiras[carreira - 1].paragens_normal,
                          paragem);
    } else {
        stored_carreiras[carreira - 1].paragens_inverso = 
        inserir_no_final(stored_carreiras[carreira - 1].paragens_inverso,
                        paragem);
    }
    /* Incrementa o numero de paragens da carreira */
    stored_carreiras[carreira - 1].numero_paragens++;
}

/* Funcao que verifica se uma paragem passa por uma carreira (e vice-versa) */
int carreira_em_paragem(int carreira, int paragem) {
    int i, max = obter_numero_carreiras(paragem);
    for(i = 0; i < max; i++) {
        if(carreira == stored_paragens[paragem - 1].carreiras[i])
            return TRUE;
    }
    return FALSE;
}

/* Funcao que cria uma ligacao entre duas paragens */
void criar_lig(char carreira_nome[], char nome_paragem1[], 
               char nome_paragem2[], Reais custo, Reais duration) {

    int numero_paragem1, numero_paragem2;
    int numero_carreira = carreira_existe(carreira_nome);
    Carreira carreira;

    /* Verifica os erros para ver se eh possivel criar a lig */
    if(!verificar_lig(carreira_nome, nome_paragem1,
                      nome_paragem2, custo, duration))
        return;

    carreira = obter_carreira_por_id(numero_carreira);

    numero_paragem1 = paragem_existe(nome_paragem1);
    numero_paragem2 = paragem_existe(nome_paragem2);

    /*
        Se a carreira nao tiver paragens, define as paragens da carreira
        como sendo a paragem 1 (origem) e a paragem 2 (destino)
        Caso contrario, verifica se a paragem 1 eh a paragem de destino
        da carreira, se for adiciona a paragem 2 no fim da carreira
        Caso contrario adiciona a paragem 1 no inicio da carreira
    */
    if(!obter_numero_paragens(numero_carreira)) {
        definir_paragens_carreira(numero_carreira, numero_paragem1,
                                  numero_paragem2);

    } else if(carreira.paragens_inverso->id == numero_paragem1) {
        adicionar_paragens_carreira(numero_carreira, numero_paragem2, FALSE);
    } else {
        adicionar_paragens_carreira(numero_carreira, numero_paragem1, TRUE);
    }

    stored_carreiras[numero_carreira - 1].custo += custo;
    stored_carreiras[numero_carreira - 1].duration += duration;
}


/* FUNCAO A SER MODIFICADA PARA O TESTE PRATICO */

/* 
    Funcao que encarrega de tratar do comando c conforme os argumentos 
    fornecidos pelo utilizador 
*/
void processar_carreiras() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos, carreira_id, i;
    Carreira carreira_pedida;
    leLinha(argumentos, &n_argumentos);

    /* 
        Caso o numero de argumentos seja 0 esta vai imprimir para todas as 
        carreiras as suas informacoes 
    */
    if(n_argumentos == 0) {
        for(i = 0; i < N_carreiras; i++) {
            Paragem comeco, fim;
            Carreira carreira = stored_carreiras[i];
            /*
                Caso a carreira tenha paragens, esta vai imprimir o nome da 
                paragem de inicio e de fim, o numero de paragens, o custo
                e a duracao
            */

           /* 
                Teste pratico - se a carreira tiver mais do que ou 6 letras nao
                eh impressa
           */
           if(strlen(carreira.nome) >= 6)
                continue;

            if(obter_numero_paragens(i+1)) {
                comeco=obter_paragem_por_id(carreira.paragens_normal->id);
                fim = obter_paragem_por_id(carreira.paragens_inverso->id);

                printf("%s %s %s %d %.2f %.2f\n", carreira.nome,
                comeco.nome, fim.nome,
                obter_numero_paragens(i+1),
                carreira.custo, carreira.duration);
            } else {
                /*
                    Caso contrario ele so imprime o nome da carreira, o numero 
                    de paragens, o custo e a duracao
                */
                printf("%s %d %.2f %.2f\n", carreira.nome,
                0, carreira.custo, carreira.duration);
            }
        }
    } else if(n_argumentos > 0) {
        carreira_id = carreira_existe(argumentos[0]);
        /* Quando a carreira nao existe cria uma nova */
        if(!carreira_id) {
            criar_carreira(argumentos[0]);
            return;
        } else {
            carreira_pedida = obter_carreira_por_id(carreira_id);
            /* 
                Caso o numero de argumentos seja 1, esta vai imprimir a 
                carreira pedida na ordem normal, caso contrario, se o segundo
                argumento for uma opcao validade de "inverso", esta vai imprimir
                a carreira pedida na ordem inversa.
            */
            if(n_argumentos == 1) {
                if(carreira_pedida.numero_paragens > 0)
                    print_lista_normal(carreira_pedida.paragens_normal);
                return;
            } else if(n_argumentos == 2 && eh_inverso(argumentos[1])) {
                if(carreira_pedida.numero_paragens > 0)
                    print_lista_inverso(carreira_pedida.paragens_inverso);
                return;
            } else {
                printf("incorrect sort option.\n");
                return;
            }
        }
    }

}

/* 
    Funcao que encarrega de tratar do comando p conforme os argumentos
    fornecidos pelo utilizador */
void processar_paragens() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos, i;

    leLinha(argumentos, &n_argumentos);
    
    /* 
        Se nao tem argumentos vai listar todas as paragens
        (o nome, a a latitude, a longitude e o numero de carreiras)
    */
    if(n_argumentos == 0) {        
        for (i = 0; i < N_paragens; i++) {
            printf("%s: %16.12f %16.12f %d\n", stored_paragens[i].nome, 
            stored_paragens[i].latitude, stored_paragens[i].longitude, 
            stored_paragens[i].numero_carreiras);
        }
        return;
    } else if(n_argumentos == 1) {
        Paragem paragem_solicitada;
        int id = paragem_existe(argumentos[0]);
        /* Se a carreira nao existe retorna erro */
        if(!id) {
            printf("%s: no such stop.\n", argumentos[0]);
            return;
        }
        paragem_solicitada = obter_paragem_por_id(id);
        /* Se a carreira existe imprime a latitude e a longitude */
        printf("%16.12f %16.12f\n", paragem_solicitada.latitude, 
               paragem_solicitada.longitude);
        return;
        
    } else if(n_argumentos == 3) {
        /* 
            Se o numero de argumentos for 3 ele cria uma paragem 
            (caso nao tenha erros no input) 
        */
        criar_paragem(argumentos[0], atof(argumentos[1]), atof(argumentos[2]));
    }
}

/* 
    Funcao que encarrega de tratar do comando l conforme os argumentos
    fornecidos pelo utilizador 
*/	
void processar_ligacao() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;
    leLinha(argumentos, &n_argumentos);
    /*
        Se o numero de argumentos for 5 ele cria uma ligacao
        (caso nao tenha erros no input)
    */
    if(n_argumentos == 5)
        criar_lig(argumentos[0], argumentos[1], argumentos[2],
        strtod(argumentos[3], NULL), strtod(argumentos[4], NULL));
}

/* 
    Algoritmo de bubbleSort para organizar a lista das carreiras de uma
    paragem por ordem alfabetica
*/
void bubbleSort(int lista[], int size) {
    int i, j, temp;
    Carreira carreira1, carreira2;
    
    for(i = 0; i < size; i++) {
        for(j = 0; j < size - 1; j++) {
            carreira1 = obter_carreira_por_id(lista[j]);
            carreira2 = obter_carreira_por_id(lista[j+1]);
            /* 
                A ordenacao e feita por ordem alfabetica entao vai a 
                procura do nome das carreiras (a atual e a seguinte)
                e compara-as 
            */
            if(strcmp(carreira1.nome, carreira2.nome) > 0) {
                temp = lista[j];
                lista[j] = lista[j+1];
                lista[j+1] = temp;
            }
        }
    }
}

/* 
    Funcao que imprime a lista de carreiras de uma paragem por ordem
    alfabetica 
*/
void print_interlig(int lista[], int size) {
    int i;
    /* Ordena a lista de carreiras por ordem alfabetica usando bubble sort*/
    bubbleSort(lista, size);
    /* Depois imprime a lista de carreiras */
    for(i = 0; i < size; i++) {
        if (i == (size - 1)) {
            printf("%s\n", stored_carreiras[lista[i]-1].nome);
            return;
        }
        printf("%s ", stored_carreiras[lista[i]-1].nome);
    }
}

/*
    Funcao que encarrega de tratar do comando i conforme os argumentos 
    fornecidos pelo utilizador 
*/
void processar_interligacao() {
    int i;
    for(i = 0; i<N_paragens; i++) {
        if(stored_paragens[i].numero_carreiras <= 1)
            continue;
        /* Faz primeiro o print do nome da paragem e o numero de carreiras*/
        printf("%s %d: ", stored_paragens[i].nome, 
        stored_paragens[i].numero_carreiras);
        /* Depois imprime a lista de carreiras por ordem alfabetica*/
        print_interlig(stored_paragens[i].carreiras, 
        stored_paragens[i].numero_carreiras);
    }
}

/* FUNCAO A SER CRIADA - TESTE PRATICO */
void processar_fim() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos, id, i, primeiro = TRUE;
    leLinha(argumentos, &n_argumentos);
    /* Se o numero de argumentos for um, entao */
    if(n_argumentos == 1) {
        id = paragem_existe(argumentos[0]);
        /* Verifica se a paragem existe */
        if(!id) {
            printf("%s: no such stop.\n", argumentos[0]);
            return;
        }
        /* 
            Se a paragem existe, vai entrar num for para ver todas as carreiras
            que tem essa paragem destino 
        */
        for(i = 0; i < N_carreiras; i++) {
            /* Se a carreira tiver paragens e a ultima for a pedida */
            if((stored_carreiras[i].numero_paragens > 0) &
               (stored_carreiras[i].paragens_inverso->id == id)) {
                /* Imprime o nome da carreira */
                if(primeiro) {
                    printf("%s", stored_carreiras[i].nome);
                    primeiro = FALSE;
                    continue;
                }
                /* 
                    Se for a primeira imprime so o nome, de resto imprime 
                    tambem um espaco
                */
                printf(" %s", stored_carreiras[i].nome);
            }
        }
        /* 
            Se primeiro tiver valor falso isso significa que imprimiu o nome de
            uma carreira, logo no final tera de imprimir uma nova linha
        */
        if(!primeiro)
            printf("\n");
    }
}