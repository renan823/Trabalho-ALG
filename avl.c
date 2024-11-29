#include "item.h"
#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definição da ED ----------------------------
typedef struct no NO_AVL;

struct no {
    ITEM *item;
    NO_AVL *esq;
    NO_AVL *dir;
    int fator;
};

struct avl {
    NO_AVL *raiz;
    int profundidade;
};

// Auxiliares ---------------------------------

int _avl_altura_no(NO_AVL *no) {
    if (no == NULL) {
        return(0);
    }

    int esq = 0;
    int dir = 0;

    esq += _avl_altura_no(no->esq);
    dir += _avl_altura_no(no->dir);

    if (dir > esq) {
        return(dir+1);
    }

    return(esq+1);
}

/*
Rotaciona a sub-árvore para a direita.
A raiz da sureturn(true)b-arvore é passada por parâmetro.
O nó rotacionado é retornado.
*/
NO_AVL *_avl_rotacionar_dir(NO_AVL *raiz) {
    NO_AVL *sub = raiz->esq;

    raiz->esq = sub->dir;
    sub->dir = raiz;

    //Normalizar fatores

    return(sub);
}

/*
Aloca um nó.
Recebe o item que será armazenado.
Os valores são inicializados se no != NULL.
Um nó sempre começa sem filhos e com fator = 0;
O ponteiro do nó é retornado.
*/
NO_AVL *_avl_criar_no(ITEM *item) {
    NO_AVL *no = (NO_AVL*) malloc(sizeof(NO_AVL));

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
void _avl_apagar_no(NO_AVL *raiz) {
    if (raiz == NULL) {
        return;
    }

    _avl_apagar_no(raiz->esq);
    _avl_apagar_no(raiz->dir);

    item_apagar(&raiz->item);

    free(raiz);
}

void _avl_inserir_no(NO_AVL **raiz, NO_AVL *no) {
    if (*raiz == NULL) {
        *raiz = no;
        return;
    }


}

/*
Exibe os nós na árvore.
Recebe a raiz como parâmetro.
Realiza o percurso em ordem recursivamente.
Nada é retornado
*/
void _avl_imprimir_em_ordem(NO_AVL *raiz) {
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
Insere um nó na árvore.
A árvore e o item são parâmetros.
Cria um novo nó e coloca-o em sua posição.
Usa uma função auxiliar para fazer a inserção
O retorno é verdadeiro ou falso para a inserção.
*/
bool avl_inserir(AVL *arvore, ITEM *item) {
    if (arvore == NULL) {
        return(false);
    }

    NO_AVL *no = _avl_criar_no(item);
    if (no == NULL) {
        return(false);
    }

    _avl_inserir_no(&arvore->raiz, no);

    return(true);
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