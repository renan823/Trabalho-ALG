#include "item.h"
#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definição da ED ----------------------------
typedef struct no NO;

struct no {
    ITEM *item;
    NO *esq;
    NO *dir;
    int fator;
};

struct avl {
    NO *raiz;
    int profundidade;
};

// Auxiliares ---------------------------------

/*
Aloca um nó.
Recebe o item que será armazenado.
Os valores são inicializados se no != NULL.
Um nó sempre começa sem filhos e com fator = 0;
O ponteiro do nó é retornado.
*/
NO *_avl_criar_no(ITEM *item) {
    NO *no = (NO*) malloc(sizeof(NO));

    if (no != NULL) {
        no->item = item;
        no->esq = NULL;
        no->dir = NULL;
        no->fator = 0;
    }

    return(no);
}

/*
Apaga recursivamente nós.
Recebe um nó inicial (raiz).
Para cada nó, seus filhos são visitados recursivamente (pós ordem).
Os items também são apagados no processo.
Nada é retornado.
*/
void _avl_apagar_no(NO *raiz) {
    if (raiz == NULL) {
        return;
    }

    _avl_apagar_no(raiz->esq);
    _avl_apagar_no(raiz->dir);

    item_apagar(&raiz->item);

    free(raiz);
}

/*
Exibe os nós na árvore.
Recebe a raiz como parâmetro.
Realiza o percurso em ordem recursivamente.
Nada é retornado
*/
void _avl_imprimir_em_ordem(NO *raiz) {
    if (raiz == NULL) {
        return;
    }

    _avl_imprimir_em_ordem(raiz->esq);
    printf("%d\n", item_get_chave(raiz->item));
    _avl_imprimir_em_ordem(raiz->dir);
}

// Interface ----------------------------------

/*
Aloca uma árvore AVL.
Não possui parâmetros.
Os valores são inicializados se arvore != NULL.
O ponteiro da árvore é retornado.
*/ 
AVL *avl_criar(void) {
    AVL *arvore = (AVL*) malloc(sizeof(AVL));

    if (arvore != NULL) {
        arvore->raiz = NULL;
        arvore->profundidade -1;
    }

    return(arvore);
}

/*
Apaga uma árvore AVL.
Recebe o endereço do ponteiro da árvore.
Todos os nós (e seus valores) serão apagados.
Essa função usa outra função para apagar os nós.
Nada é retornado.
*/
void avl_apagar(AVL **arvore) {
    if (*arvore == NULL) {
        return;
    }

    //Apagar nós
    _avl_apagar_no((*arvore)->raiz);

    free(*arvore);
    *arvore = NULL;
}

/*
Imprime todos os nós na árvore.
Recebe a árvore a ser exibida.
Usa uma função auxiliar para fazer o percurso em ordem
Nada é retornado.
*/
void avl_imprimir(AVL *arvore) {
    if (arvore == NULL) {
        return;
    }

    _avl_imprimir_em_ordem(arvore->raiz);
}