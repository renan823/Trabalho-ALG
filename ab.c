#include "item.h"
#include "ab.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//Definição das estruturas usadas -------------------------------------------------


typedef struct _no NO;

struct _no {
    ITEM *item;
    NO *esq;
    NO *dir;
};

struct arv_bin {
    NO *raiz;
    int profundidade;
};


//Funções auxiliares, não estão na interface --------------------------------------


/*
Função recursiva que apaga os nós e seus items
*/
void _ab_apagar_no(NO **no) {
    if (*no == NULL) {
        return;
    }

    //verifica os filhos
    _ab_apagar_no(&(*no)->esq);
    _ab_apagar_no(&(*no)->dir);

    //apaga o item
    item_apagar(&(*no)->item);

    //apaga o no
    free(*no);
    *no = NULL;
}

/*
Função para criar um no
Se o no não for nulo, seu valor é inicializado.
*/
NO *_criar_no(ITEM *item) {
    NO *no = (NO*) malloc(sizeof(NO));
    if (no != NULL) {
        no->item = item;
        no->esq = NULL;
        no->dir = NULL;
    }

    return(no);
}

/*
Função recursiva que procura o local de inserção informado e coloca o nó
Usa uma adaptação do percurso "pos-order"
*/
void _inserir_no(NO *raiz, NO *no, int pai, int lado) {
    if (raiz == NULL) {
        return;
    }

    _inserir_no(raiz->esq, no, pai, lado);
    _inserir_no(raiz->dir, no, pai, lado);

    if (item_get_chave(raiz->item) == pai) {
        if (lado == FILHO_ESQ) {
            raiz->esq = no;
        }

        if (lado == FILHO_DIR) {
            raiz->dir = no;
        }
    }
}

/*
Função que exibe os nós no percurso "in-order"
Sequencia: esquerda, raiz, direita
*/
void _percurso_ordem(NO *no) {
    if (no == NULL) {
        return;
    }

    _percurso_ordem(no->esq);
    printf("%d\n", item_get_chave(no->item));
    _percurso_ordem(no->dir);
}

/*
Função que verifica se a ab é estritamente binaria.
Usa algo parecido com o percurso, mas sempre verificando quantos filhos tem um no.
Ao encontrar um nó com apenas um 1 filho, o retorno é imediato
*/
int _estritamente_binaria(NO *no) {
    if (no == NULL) {
        return(0);
    }

    if ((no->esq == NULL && no->dir != NULL) || (no->esq != NULL && no->dir == NULL)) {
        return(1);
    }

    int esq = _estritamente_binaria(no->esq);
    int dir = _estritamente_binaria(no->dir);

    if (esq == 0 && dir == 0) {
        return(0);
    }

    return(1);
}

//Funções principais --------------------------------------------------------------


/*
Função que aloca uma AB. 
Se a AB não for nula, seus valores são inicializados
*/
AB *ab_criar(void) {
    AB *arvore = (AB*) malloc(sizeof(AB));
    if (arvore != NULL) {
        arvore->raiz = NULL;
        arvore->profundidade = -1;
    }

    return(arvore);
}

/*
Função que apaga uma AB e seus nós.
Os nós são apagados com uma função auxiliar recursiva.
*/
void ab_apagar(AB **arvore) {
    if (*arvore == NULL) {
        return;
    }

    _ab_apagar_no(&(*arvore)->raiz);

    free(*arvore);
    *arvore = NULL;
}

/*
Função para inserir um nó na arvore.
Para a inserção é necessário o novo valor, a chave do nó pai e o lado (esq/dir)
*/
bool ab_inserir(AB *arvore, ITEM *item, int lado, int chave) {
    if (arvore == NULL) {
        return(false);
    }

    NO *no = _criar_no(item);
    if (no == NULL) {
        return(false);
    }

    if (arvore->raiz == NULL) {
        arvore->raiz = no;
        return(true);
    }

    _inserir_no(arvore->raiz, no, chave, lado);
    return(true);
}

/*
Imprime os nós na ordem esquerda, raiz, direita.
Usa uma função auxiliar
*/
void ab_imprimir(AB *arvore) {
    _percurso_ordem(arvore->raiz);
}

/*
Função que verifica se a arvore é estritamente binaria
Retorna 0 se for estritatemente binaria e 1 caso contrário
Usa uma função auxiliar recursiva
*/
int ab_estritamente_binaria (AB *arvore) {
    return _estritamente_binaria(arvore->raiz);
}