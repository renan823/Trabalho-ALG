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
void _avl_rotacionar_dir(NO_AVL **raiz) {
    NO_AVL *filho = (*raiz)->esq;
    // Passar filho->dir para pai->esq;
    (*raiz)->esq = filho->dir;

    // Filho substitui pai
    filho->dir = (*raiz);

    //Normalizar fatores
    (*raiz)->fator = _avl_altura_no((*raiz)->esq) - _avl_altura_no((*raiz)->dir);
    filho->fator = _avl_altura_no(filho->esq) - _avl_altura_no(filho->dir);

    // Substituir raiz
    *raiz = filho;
}

void _avl_rotacionar_esq(NO_AVL **raiz) {
    NO_AVL *filho = (*raiz)->dir;

    // Passar filho->esq para pai->dir;
    (*raiz)->dir = filho->esq;

    // Filho substitui pai
    filho->esq = *raiz;

    //Normalizar fatores
    (*raiz)->fator = _avl_altura_no((*raiz)->esq) - _avl_altura_no((*raiz)->dir);
    filho->fator = _avl_altura_no(filho->esq) - _avl_altura_no(filho->dir);

    *raiz = filho;
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

/*
*/
void _avl_inserir_no(NO_AVL **raiz, NO_AVL *no) {
    // Caso 1 - Raiz nula
    if (*raiz == NULL) {
        *raiz = no;
        return;
    }

    if (item_get_chave((*raiz)->item) > item_get_chave(no->item)) {
        _avl_inserir_no(&(*raiz)->esq, no);
    }

    if (item_get_chave((*raiz)->item) < item_get_chave(no->item)) {
        _avl_inserir_no(&(*raiz)->dir, no);
    }

    if (item_get_chave((*raiz)->item) == item_get_chave(no->item)) {
        return;
    }

    // Normalizar fator
    (*raiz)->fator = _avl_altura_no((*raiz)->esq) - _avl_altura_no((*raiz)->dir);

    // Inseriu na direita
    if ((*raiz)->fator == -2) {
        if ((*raiz)->dir->fator <= 0) {
            // Sinais iguais - Rot. Simples Esq
            _avl_rotacionar_esq(raiz);
        } else {
            // Sinais diferentes - Rot. Dupla Dir/Esq
            _avl_rotacionar_dir(&(*raiz)->dir);
            _avl_rotacionar_esq(raiz);
        }   
    }

    // Inseriu na esquerda
    if ((*raiz)->fator == 2) {
        if ((*raiz)->esq->fator >= 0) {
            // Sinais iguais - Rot. Simples Dir
            _avl_rotacionar_dir(raiz);
        } else {
            // Sinais diferentes - Rot. Dupla Esq/Dir
            _avl_rotacionar_esq(&(*raiz)->esq);
            _avl_rotacionar_dir(raiz);
        }   
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

/*
Copia todos os nós de uma arvore para outra.
A nova árvore e a raiz da arvore a ser 
copiada são parâmetros.
Primeiro insere o item na raiz, depois esq e dir.
Nada é retornado.
*/
void _avl_copiar_no(AVL **copia, NO_AVL *raiz) {
    if (*copia == NULL || raiz == NULL) {
        return;
    }

    _avl_inserir_no(&(*copia)->raiz, _avl_criar_no(raiz->item));
    _avl_copiar_no(copia, raiz->esq);
    _avl_copiar_no(copia, raiz->dir);
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
*/
ITEM *avl_remover(AVL *arvore, int chave) {
    if (arvore == NULL) {
        return(NULL);
    }

    return(NULL);
}

ITEM *avl_buscar(AVL *arvore, int chave) {
    if (arvore == NULL) {
        return(NULL);
    }

    return(NULL);
}

/*
*/
AVL *avl_unir(AVL *a1, AVL *a2) {
    if (a1 == NULL || a2 == NULL) {
        return(NULL);
    }

    AVL *arvore = avl_criar();
    _avl_copiar_no(&arvore, a1->raiz);
    _avl_copiar_no(&arvore, a2->raiz);

    return(arvore);
}

/*
*/
AVL *avl_intersectar(AVL *a1, AVL *a2) {
    if (a1 == NULL || a2 == NULL) {
        return(NULL);
    }

    AVL *arvore = avl_criar();
   

    return(arvore);
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

    if (arvore->raiz == NULL) {
        printf("Vazia!\n");
    }

    _avl_imprimir_em_ordem(arvore->raiz);
}