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
Calcula a altura de um nó.
O nó é o parâmetro.
Pecorre as sub-árvores e soma a altura.
Compara a altura direita com a esquerda.
Retorna a maior altura (direita ou esquerda)
*/
int _avl_altura_no(NO_AVL *no) {
    if (no == NULL) {
        return 0;
    }

    int altura_esq = _avl_altura_no(no->esq);
    int altura_dir = _avl_altura_no(no->dir);

    return (altura_esq > altura_dir ? altura_esq : altura_dir) + 1;
}

/*
Rotaciona a sub-árvore para a direita.
A raiz da sub-arvore é passada por parâmetro.
Ajusta ponteiros e coloca na raiz.
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

/*
Rotaciona a sub-árvore para a esquerda.
A raiz da sub-arvore é passada por parâmetro.
Ajusta ponteiros e coloca na raiz.
O nó rotacionado é retornado.
*/
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
Busca por uma chave.
O nó inicial e a chave são parâmetros.
A busca binária percorre caminhos até achar o valor.
Se o item é achado, ele será retornado.
Caso contrário, nulo é retornado.
*/
ITEM *_avl_busca_binaria(NO_AVL *no, int chave) {
    if (no == NULL) {
        return(NULL);
    }

    if (item_get_chave(no->item) > chave) {
        return(_avl_busca_binaria(no->esq, chave));
    }

    if (item_get_chave(no->item) < chave) {
        return(_avl_busca_binaria(no->dir, chave));
    }

    return(no->item);
}

/*
Encontra o maior nó esquerdo e troca com a raiz.
A raiz e o filho inicial são parâmetros.
Na chamada da função, o filho deve ser o esquerdo.
Ao encontrar o maior filho (sem filho direito), 
troque seu valor com a raiz.
O ajuste de sub-aŕvore esquerda (se existir) é feito.
O Nó é liberado.
Nada é retornado.
*/
void _avl_trocar_maximo_esq(NO_AVL **raiz, NO_AVL **filho) {
    // Acha o maior filho direito
    if ((*filho)->dir != NULL) {
        _avl_trocar_maximo_esq(raiz, &(*filho)->dir);
    }

    // Troca o maior filho com a raiz
    (*raiz)->item = (*filho)->item;

    //Ajustar possiveis filhos e remover
    NO_AVL *temp = *filho;

    *filho = (*filho)->esq;

    free(temp);
}

/*
Insere um novo nó na árvore.
O endereço da raiz da árvore e o nó são parâmetros.
Caso base: Raiz vazia, insira nela.
Caso geral: Busque a posição e insira ali.
Caso o fator esteja errado, rotações são feitas.
Nada é retornado.
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
Remove um nó.
A referencia da raiz e a chave de remoção
são parâmetros.
Busca pela chave e remove o nó (se existir).
Caso 1: Nó é folha, basta remover
Caso 2: Nó tem só um filho, basta ajustar o outro filho
e depois remover
Caso 3: Nó tem ambos os filhos, pega o maior da esquerda
substitui na raiz, ajusta os filhos e remove.
Após remoção, os fatores são calculados novamente.
Retorna o nó removido ou NULL.
*/
ITEM *_avl_remover_no(NO_AVL **raiz, int chave) {
    if (*raiz == NULL) {
        return(NULL);
    }

    ITEM *valor = NULL;

    if (item_get_chave((*raiz)->item) > chave) {
        valor = _avl_remover_no(&(*raiz)->esq, chave);
    }

    if (item_get_chave((*raiz)->item) < chave) {
        valor = _avl_remover_no(&(*raiz)->dir, chave);
    }

    if (item_get_chave((*raiz)->item) == chave) {
        valor = (*raiz)->item;

        // Caso 1 e 2 - Nó folha ou filho único
        if ((*raiz)->esq == NULL || (*raiz)->dir == NULL) {
            NO_AVL *temp = NULL;

            if ((*raiz)->esq == NULL) {
                temp = (*raiz)->dir;
            } else {
                temp = (*raiz)->esq;
            }

            free(*raiz);
            *raiz = temp;
        } else {
            // Caso 3: Nó tem os dois filhos
            _avl_trocar_maximo_esq(raiz, &(*raiz)->esq);
        }

        // Ajustar balanceamento após remover (igual na inserção)
        if (*raiz == NULL) {
            return(valor);
        }
        
        (*raiz)->fator = _avl_altura_no((*raiz)->esq) - _avl_altura_no((*raiz)->dir);

        // Removeu na direita
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

        // Removeu na esquerda
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

    return(valor);
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
    printf("%d, ", item_get_chave(raiz->item));
    _avl_imprimir_em_ordem(raiz->dir);
}

/*
Copia todos os nós de uma arvore para outra.
A nova árvore e a raiz da arvore a ser 
copiada são parâmetros.
Primeiro insere o item na raiz, depois esq e dir.
Nada é retornado.
*/
void _avl_copiar(AVL **copia, NO_AVL *raiz) {
    if (*copia == NULL || raiz == NULL) {
        return;
    }

    _avl_inserir_no(&(*copia)->raiz, _avl_criar_no(raiz->item));
    _avl_copiar(copia, raiz->esq);
    _avl_copiar(copia, raiz->dir);
}

/*
Copia os nós que estão em ambas árvores para a nova.
A nova árvore e as raizes das arvores a
serem copiadas são parâmetros.
Verifica se o elemento de a1 esta em a2 e
adiciona na nova arvore se estiver.
Para buscar pela chave, usa a função de busca binária
Nada é retornado.
*/
void _avl_combinar(AVL **copia, NO_AVL *a1, NO_AVL *a2) {
    if (*copia == NULL || a1 == NULL || a2 == NULL) {
        return;
    }

    // Verifica se esta em ambas
    if (_avl_busca_binaria(a2, item_get_chave(a1->item)) != NULL) {
        _avl_inserir_no(&(*copia)->raiz, _avl_criar_no(a1->item));
    }

    _avl_combinar(copia, a1->esq, a2);
    _avl_combinar(copia, a1->dir, a2);
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
Remove um nó pela chave.
A arvore alvo e a chave de remoção são parâmetros.
Usa uma função auxiliar para buscar e remover.
Retorna o item removido ou NULL
*/
ITEM *avl_remover(AVL *arvore, int chave) {
    if (arvore == NULL) {
        return(NULL);
    }

    return(_avl_remover_no(&arvore->raiz, chave));
}

/*
Busca uma determinada chave.
A arvore alvo e a chave de busca são parâmetros.
Usa uma função de busca binaria auxiliar.
Retorna o item encontrado ou nulo.
*/
ITEM *avl_buscar(AVL *arvore, int chave) {
    if (arvore == NULL) {
        return(NULL);
    }

    return(_avl_busca_binaria(arvore->raiz, chave));
}

/*
Junta duas árvores.
As duas árvores são parâmetros.
Usa uma função auxiliar para realizar a união.
A nova árvore criada é retornada.
*/
AVL *avl_unir(AVL *a1, AVL *a2) {
    if (a1 == NULL || a2 == NULL) {
        return(NULL);
    }

    AVL *arvore = avl_criar();
    _avl_copiar(&arvore, a1->raiz);
    _avl_copiar(&arvore, a2->raiz);

    return(arvore);
}

/*
Cria uma árvore cujos items estão 
nas duas árvores indicadas.
As duas árvores são parâmetros.
Usa uma função auxiliar para combinar.
A nova árvore é retornada.
*/
AVL *avl_intersectar(AVL *a1, AVL *a2) {
    if (a1 == NULL || a2 == NULL) {
        return(NULL);
    }

    AVL *arvore = avl_criar();
    _avl_combinar(&arvore, a1->raiz, a2->raiz);

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