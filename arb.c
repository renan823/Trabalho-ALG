#include "item.h"
#include "arb.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definição das cores
#define RED 'r'
#define BLACK 'b'

// Definição da ED ----------------------------
typedef struct no NO_ARB;

struct no {
    ITEM *item;
    NO_ARB *esq;
    NO_ARB *dir;
    char cor;
};

struct arb {
    NO_ARB *raiz;
    int profundidade;
};

// Auxiliares ---------------------------------

/*
Aloca um nó.
Recebe o item como parâmetro.
Caso o nó seja != nulo, seus valores são inicializados.
Todo nó começa sem filhos.
Todo nó inicia vermelho!
O nó é retornado.
*/
NO_ARB *_arb_criar_no(ITEM *item) {
    NO_ARB *no = (NO_ARB*) malloc(sizeof(NO_ARB));

    if (no != NULL) {
        no->item = item;
        no->esq = NULL;
        no->dir = NULL;
        no->cor = RED;
    }

    return(no);
}

/*
Apaga nós.
Recebe o nó alvo como parâmetro.
Busca recursivamente todos os nós, apagando-os.
Os items também são apagados.
Nada é retornado.
*/
void _arb_apagar_no(NO_ARB *no) {
    if (no == NULL) {
        return;
    }

    _arb_apagar_no(no->esq);
    _arb_apagar_no(no->dir);

    item_apagar(&no->item);
    free(no);
}

/*
Visita nós em ordem.
Recebe o nó inicial por parâmetro.
Percorre na ordem esq - raiz - dir.
Exibe a chave de cada nó.
Nada é retornado.
*/
void _arb_percurso_ordem(NO_ARB *no) {
    if (no == NULL) {
        return;
    }

    _arb_percurso_ordem(no->esq);
    printf("%d\n", item_get_chave(no->item));
    _arb_percurso_ordem(no->dir); 
}

/*
Busca por uma chave.
O nó inicial e a chave são parâmetros.
A busca binária percorre caminhos até achar o valor.
Se o item é achado, ele será retornado.
Caso contrário, nulo é retornado.
*/
ITEM *_arb_busca_binaria(NO_ARB *no, int chave) {
    if (no == NULL) {
        return(NULL);
    }

    if (item_get_chave(no->item) > chave) {
        return(_arb_busca_binaria(no->esq, chave));
    }

    if (item_get_chave(no->item) < chave) {
        return(_arb_busca_binaria(no->dir, chave));
    }

    return(no->item);
}

// Interface ----------------------------------

/*
Cria uma árvore rubro-negra.
Não possui parâmetros.
Caso arvore != nulo, os valores são inicializados.
A profundidade começa em -1. A raiz é nula.
*/
ARB *arb_criar(void) {
    ARB *arvore = (ARB*) malloc(sizeof(ARB));

    if (arvore != NULL) {
        arvore->profundidade = -1;
        arvore->raiz = NULL;
    }

    return(arvore);
}

/*
Apaga uma árvore rubro-negra
A referência para árvore é passada por parâmetro.
Usa uma função auxiliar para apagar os nós.
Nada é retornado.
*/
void arb_apagar(ARB **arvore) {
    if (*arvore == NULL) {
        return;
    }

    //apagar nós
    _arb_apagar_no((*arvore)->raiz);

    //apagar árvore
    free(*arvore);
    *arvore = NULL;
}

/*
*/
bool arb_inserir(ARB *arvore, ITEM *item) {
    if (arvore == NULL) {
        return(false);
    }

    return(true);
}

/*
Busca um item pela chave dada.
A árvore alvo e a chave de busca são parâmetros.
Usa uma função auxiliar para buscar.
O retorno é o item ou nulo.
*/
ITEM *arb_buscar(ARB *arvore, int chave) {
    if (arvore == NULL) {
        return(NULL);
    }

    return(_arb_busca_binaria(arvore->raiz, chave));
}

/*
Exibe os nós na árvore.
A árvore alvo é o parâmetro.
Usa uma função auxiliar para percorrer a árvore.
Nada é retornado.
*/
void arb_imprimir(ARB *arvore) {
    if (arvore == NULL) {
        return;
    }

    _arb_percurso_ordem(arvore->raiz);
}
