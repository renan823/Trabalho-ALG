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
    printf("%d - %c\n", item_get_chave(no->item), no->cor);
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

void _arb_rotacionar_esq(NO_ARB **no) {
    NO_ARB *filho = (*no)->dir;
    NO_ARB *pai = *no;

    // Passar filho->esq para pai->dir;
    pai->dir = filho->esq;

    // Filho substitui pai
    filho->esq = pai;

    // Ajustar cores
    filho->cor = pai->cor;
    pai->cor = RED;

    // Substitui raiz
    *no = filho;
}

void _arb_rotacionar_dir(NO_ARB **no) {
    NO_ARB *filho = (*no)->esq;
    NO_ARB *pai = *no;

    // Passar filho->dir para pai->esq;
    pai->esq = filho->dir;

    // Filho substitui pai
    filho->dir = pai;

    // Ajustar cores
    filho->cor = pai->cor;
    pai->cor = RED;

    // Substitui raiz
    *no = filho;
}

/*
Inverte a cor de um nó.
Recebe a referência do nó como parâmetro.
A raiz se torna vermelha.
Os filhos se tornam pretos.
Nada é retornado.
*/
void _arb_inverter_cor(NO_ARB **no) {
    if (*no == NULL) {
        return;
    }

    (*no)->cor = RED;
    (*no)->esq->cor = BLACK;
    (*no)->dir->cor = BLACK;
}

/*
Verifica se um nó é vermelho.
Recebe o nó por parâmetro.
Garante segurança em casos que nó = NULL.
Retorna true se o nó for vermelho.
*/
bool _arb_no_vermelho(NO_ARB *no) {
    if (no == NULL) {
        return(false);
    }

    return(no->cor == RED);
}

/*
Verifica se um nó é preto.
Recebe o nó por parâmetro.
Garante segurança em casos que nó = NULL.
Nós NULL são pretos!
Retorna true se o nó for preto.
*/
bool _arb_no_preto(NO_ARB *no) {
    if (no == NULL) {
        return(true);
    }

    return(no->cor == BLACK);
}

/*
*/
void _arb_inserir_no(NO_ARB **raiz, NO_ARB *no) {
    // Caso 1 - raiz vazia
    if (*raiz == NULL) {
        *raiz = no;
        return;
    }

    // Ida recursão
    if (item_get_chave((*raiz)->item) > item_get_chave(no->item)) {
        _arb_inserir_no(&(*raiz)->esq, no);
    }

    if (item_get_chave((*raiz)->item) < item_get_chave(no->item)) {
        _arb_inserir_no(&(*raiz)->dir, no);
    }

    // Caso 2 - Filho direito RED
    if (_arb_no_vermelho((*raiz)->dir) && !_arb_no_vermelho((*raiz)->esq)) {
        _arb_rotacionar_esq(raiz);
    }

    // Caso 3.2 - RED / RED seguidos
    if (_arb_no_vermelho((*raiz)->esq) && _arb_no_vermelho((*raiz)->esq->esq)) {
        _arb_rotacionar_dir(raiz);
    }

    // Repintar nós caso ambos os filhos RED
    if (_arb_no_vermelho((*raiz)->dir) && _arb_no_vermelho((*raiz)->esq)) {
        _arb_inverter_cor(raiz);
    }
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

    NO_ARB *no = _arb_criar_no(item);
    if (no == NULL) {
        return(false);
    }

    _arb_inserir_no(&arvore->raiz, no);

    // Sempre garante a raiz preta
    arvore->raiz->cor = BLACK;

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
Remove um item pela chave especificada.
A árvore alvo e a chave do item são parâmetros.
USa uma função auxiliar para buscar e remover;
O retorno é o item ou nulo.
*/
ITEM *arb_remover(ARB *arvore, int chave) {
    if (arvore == NULL) {
        return(NULL);
    }

    return(NULL);
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
