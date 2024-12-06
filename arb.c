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

/*
Rotaciona uma sub-árvore para a esquerda.
Recebe a referência da raiz como parâmetro.
Os ponteiros são ajustados.
Os nós são repintados.
Nada é retornado.
*/
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

/*
Rotaciona uma sub-árvore para a direita.
Recebe a referência da raiz como parâmetro.
Os ponteiros são ajustados.
Os nós são repintados.
Nada é retornado.
*/
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
Encontra o maior nó esquerdo e troca com a raiz.
A raiz e o filho inicial são parâmetros.
Na chamada da função, o filho deve ser o esquerdo.
Ao encontrar o maior filho (sem filho direito), 
troque seu valor com a raiz.
O ajuste de sub-aŕvore esquerda (se existir) é feito.
O Nó é liberado.
Nada é retornado.
*/
void _arb_trocar_maximo_esq(NO_ARB **raiz, NO_ARB **filho) {
    // Acha o maior filho direito
    if ((*filho)->dir != NULL) {
        _arb_trocar_maximo_esq(raiz, &(*filho)->dir);
    }

    // Troca o maior filho com a raiz
    (*raiz)->item = (*filho)->item;

    //Ajustar possiveis filhos e remover
    NO_ARB *temp = *filho;

    *filho = (*filho)->esq;

    free(temp);
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
Insere um nó.
A referencia para a raiz e o novo nó
são passados por parâmetro.
Caso 1: A raiz é vazia, insira nela.
Caso 2: Filho direito RED, rotacionar esquerda
Caso 3: Nós RED seguidos, rotacionar direita
Caso 4 (ajuste): Se esq e dir são RED, inverter cores e
propagar a inversão na recursão.
Quem chama a função tem como abrigação garantir que
a raiz sempre seja BLACK.
Nada é retornado.
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

    // Caso 3 - RED / RED seguidos
    if (_arb_no_vermelho((*raiz)->esq) && _arb_no_vermelho((*raiz)->esq->esq)) {
        _arb_rotacionar_dir(raiz);
    }

    // Repintar nós caso ambos os filhos RED
    if (_arb_no_vermelho((*raiz)->dir) && _arb_no_vermelho((*raiz)->esq)) {
        _arb_inverter_cor(raiz);
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
Após remoção a árvore é validada e realiza rotações
e trocas de cor quando necessário.
Retorna o nó removido ou NULL.
*/
ITEM *_arb_remover_no(NO_ARB **raiz, int chave) {
    if (*raiz == NULL) {
        return(NULL);
    }

    ITEM *valor = NULL;

    if (item_get_chave((*raiz)->item) > chave) {
        valor = _arb_remover_no(&(*raiz)->esq, chave);
    }

    if (item_get_chave((*raiz)->item) < chave) {
        valor = _arb_remover_no(&(*raiz)->dir, chave);
    }

    // Chave encontrada
    if (item_get_chave((*raiz)->item) == chave) {
        valor = (*raiz)->item;

        // Caso 1 e 2 - Nó folha ou filho único
        if ((*raiz)->esq == NULL || (*raiz)->dir == NULL) {
            NO_ARB *temp = NULL;

            if ((*raiz)->esq == NULL) {
                temp = (*raiz)->dir;
            } else {
                temp = (*raiz)->esq;
            }

            if (_arb_no_preto(*raiz) && _arb_no_vermelho(temp)) {
                temp->cor = BLACK;
            }
            
            // Remover nó
            free(*raiz);
            *raiz = temp;
        } else {
            // Caso 3: Nó tem os dois filhos
            _arb_trocar_maximo_esq(raiz, &(*raiz)->esq);
        }

        // Ajustar balanceamento após remover (igual na inserção)
        if (*raiz == NULL) {
            return(valor);
        }
        
        // Filho direito RED
        if (_arb_no_vermelho((*raiz)->dir) && !_arb_no_vermelho((*raiz)->esq)) {
            _arb_rotacionar_esq(raiz);
        }

        // RED / RED seguidos
        if (_arb_no_vermelho((*raiz)->esq) && _arb_no_vermelho((*raiz)->esq->esq)) {
            _arb_rotacionar_dir(raiz);
        }

        // Repintar nós caso ambos os filhos RED
        if (_arb_no_vermelho((*raiz)->dir) && _arb_no_vermelho((*raiz)->esq)) {
            _arb_inverter_cor(raiz);
        }
    }

    return(valor);
}

/*
Copia todos os nós de uma arvore para outra.
A nova árvore e a raiz da arvore a ser 
copiada são parâmetros.
Primeiro insere o item na raiz, depois esq e dir.
Nada é retornado.
*/
void _arb_copiar(ARB **copia, NO_ARB *raiz) {
    if (*copia == NULL || raiz == NULL) {
        return;
    }

    _arb_inserir_no(&(*copia)->raiz, _arb_criar_no(raiz->item));
    _arb_copiar(copia, raiz->esq);
    _arb_copiar(copia, raiz->dir);
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
void _arb_combinar(ARB **copia, NO_ARB *a1, NO_ARB *a2) {
    if (*copia == NULL || a1 == NULL || a2 == NULL) {
        return;
    }

    // Verifica se esta em ambas
    if (_arb_busca_binaria(a2, item_get_chave(a1->item)) != NULL) {
        _arb_inserir_no(&(*copia)->raiz, _arb_criar_no(a1->item));
    }

    _arb_combinar(copia, a1->esq, a2);
    _arb_combinar(copia, a1->dir, a2);
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
Insere um nó na árvore.
A árvore desejada e o item são parâmetros.
Usa uma função auxiliar para inserir.
Após a inserção, garante que raiz seja BLACK.
Retorna falso se a árvore = NULL ou se não
foi possível criar (alocar) um novo nó.
Retorn true caso contrário.
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
Usa uma função auxiliar para buscar e remover;
Retorna o item removido ou nulo
*/
ITEM *arb_remover(ARB *arvore, int chave) {
    if (arvore == NULL) {
        return(NULL);
    }

    return(_arb_remover_no(&arvore->raiz, chave));
}

/*
Junta duas árvores.
As duas árvores são parâmetros.
Usa uma função auxiliar para realizar a união.
A nova árvore criada é retornada.
*/
ARB *arb_unir(ARB *a1, ARB *a2) {
    if (a1 == NULL || a2 == NULL) {
        return(NULL);
    }

    ARB *arvore = arb_criar();
    _arb_copiar(&arvore, a1->raiz);
    _arb_copiar(&arvore, a2->raiz);

    return(arvore);
}

/*
Cria uma árvore cujos items estão 
nas duas árvores indicadas.
As duas árvores são parâmetros.
Usa uma função auxiliar para combinar.
A nova árvore é retornada.
*/
ARB *arb_intersectar(ARB *a1, ARB *a2) {
    if (a1 == NULL || a2 == NULL) {
        return(NULL);
    }

    ARB *arvore = arb_criar();
    _arb_combinar(&arvore, a1->raiz, a2->raiz);

    return(arvore);
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

    if (arvore->raiz == NULL) {
        printf("Vazia!\n");
    }

    _arb_percurso_ordem(arvore->raiz);
}
