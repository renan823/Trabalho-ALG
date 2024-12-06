#include "set.h"
#include "item.h"
#include "avl.h"
#include "arb.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ED_AVL 0 //quando usar avl/rubro-negra

// Defininindo set com um inteiro para indicar o tipo e um ponteiro para o nó inicial do set
struct _set {
    int tipo;
    void *ed;
};

/**
 * Essa função cria o set, selecionando o tipo de árvore
 * @param tipo Especifica o tipo de árvore
 * @return Retorna o set criado
 */
SET *set_criar(int tipo) {
    SET *set = (SET*) malloc(sizeof(SET));

    if (set != NULL) {
        set->tipo = tipo;

        if (tipo == ED_AVL) {
            set->ed = avl_criar(); 
        } else {
            set->ed = arb_criar();
        }
    }
    return(set);
}

/**
 * Essa função apaga o set
 * @param set Recebe um ponteiro para ponteiro para o set de interesse
 */
void set_apagar(SET **set) {
    if (*set == NULL) {
        return;
    }

    if ((*set)->tipo == ED_AVL) {
        avl_apagar((AVL**) &(*set)->ed);
    } else {
        arb_apagar((ARB**) &(*set)->ed);
    }

    free(*set);
    *set = NULL;
}

/**
 * Essa função verifica se uma chave pertence ao set fornecido
 *
 * Ela usa as funções de busca de cada uma das árvores para verificar se a chave pertence ou não
 *
 * @param set Set que se quer verificar
 * @param chave Chave que quer verificar se pertence ao set
 * @return Retorna falso se não encontrar a chave e verdadeiro se encontrar
 */
bool set_pertence(SET *set, int chave) {
    if (set == NULL) {
        return(false);
    }

    if (set->tipo == ED_AVL) {
        return(avl_buscar((AVL*) set->ed, chave) != NULL);
    } else {
        return(arb_buscar((ARB*) set->ed, chave) != NULL);
    }

}

/**
 * Essa função insere um item no set
 *
 * Ela usa as funções de inserir de cada uma das árvores para poder inserir
 *
 * @param set Set no qual será inserido o item
 * @param item item que será inserido no set
 * @return Retorna verdadeiro se a inserção for bem-sucedida e falso se for mal sucedida
 */
bool set_inserir(SET *set, ITEM *item) {
    if (set == NULL) {
        return(false);
    }

    if (set->tipo == ED_AVL) {
        return(avl_inserir((AVL*) set->ed, item));
    } else {
        return(arb_inserir((ARB*) set->ed, item));
    }
}

/**
 * Essa função remove um item do set de acordo com a chave fornecida
 *
 * Ele usa as funções de remover de cada uma das árvores
 *
 * @param set Set do qual será removido o item
 * @param chave Chave do item que será removido
 * @return Retorna verdadeiro se for bem sucedida e falso se for mal sucedida
 */
bool set_remover(SET *set, int chave) {
    if (set == NULL) {
        return(false);
    }

    ITEM *resultado = NULL;

    if (set->tipo == ED_AVL) {
        resultado = avl_remover((AVL*) set->ed, chave);
    } else {
        resultado = arb_remover((ARB*) set->ed, chave);
    }

    if (resultado == NULL) {
        return(false);
    }

    item_apagar(&resultado);
    return(true);
}

/**
 * Essa função faz a união de dois sets
 *
 * Ele usa as funções unir de cada uma das árvores para poder fazer a união
 *
 * @param a Set a
 * @param b Set b
 * @return Retorna um set que é a união dos outros dois sets
 */
SET *set_uniao(SET *a, SET *b) {
    if ((a == NULL || b == NULL) || (a->tipo != b->tipo)) {
        return(NULL);
    }

    // Inicializa novo set
    SET *set = set_criar(a->tipo);

    if (a->tipo == ED_AVL) {
        set->ed = avl_unir((AVL*) a->ed, (AVL*) b->ed);
    } else {
        set->ed = arb_unir((ARB*) a->ed, (ARB*) b->ed);
    }

    return(set);
}

/**
 * Essa função encontra a interseção de dois sets e retorna um set formado por essa interseção
 *
 * Ele usa as funções intersectar das árvores para achar a interseção
 *
 * @param a Set a
 * @param b Set b
 * @return Retorna o set formado pela interseção dois dois sets
 */
SET *set_interseccao(SET *a, SET *b) {
    if ((a == NULL || b == NULL) || (a->tipo != b->tipo)) {
        return(NULL);
    }

    // Inicializa novo set
    SET *set = set_criar(a->tipo);

    if (a->tipo == ED_AVL) {
        set->ed = avl_intersectar((AVL*) a->ed, (AVL*) b->ed);
    } else {
        set->ed = arb_intersectar((ARB*) a->ed, (ARB*) b->ed);
    }

    return(set);
}

/**
 * Essa função imprime os sets
 *
 * Ela usa a função imprimir de cada uma das árvores
 *
 * @param set Set que será impresso
 */
void set_imprimir(SET *set) {
    if (set == NULL) {
        return;
    }

    if (set->tipo == ED_AVL) {
        avl_imprimir((AVL*) set->ed);
    } else {
        arb_imprimir((ARB*) set->ed);
    }
}