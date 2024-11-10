#include "item.h"
#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct no_ NO;

struct no_ {
    ITEM* item;
    NO* prox;
};

struct lista_ {
    int tamanho;
    NO* inicio;
    NO* fim;
    bool ordenada;
};

//auxiliares
void _apagar_no(NO* no) {
    if (no == NULL) {
        return;
    }

    _apagar_no(no->prox);
    item_apagar(&no->item);
    free(no);
}
//fim auxiliares

LISTA* lista_criar(bool ordenada) {
    LISTA* lista = (LISTA*) malloc(sizeof(LISTA));

    if (lista != NULL) {
        lista->ordenada = ordenada;
        lista->inicio = NULL;
        lista->fim = NULL;
    }

    return(lista);
}

void lista_apagar(LISTA** lista) {
    if (*lista == NULL) {
        return;
    }

    _apagar_no((*lista)->inicio);

    free(*lista);
    *lista = NULL;
}

bool lista_inserir(LISTA* lista, ITEM* item) {
    if (lista_cheia(lista)) {
        return(false);
    }

    //cria no
    NO* no = (NO*) malloc(sizeof(NO));
    if (no == NULL) {
        return(false);
    }

    no->item = item;
    no->prox = NULL; //por enquanto

    //caso especial: lista vazia
    if (lista_vazia(lista)) {
        lista->inicio = no;
        lista->fim = no;
        lista->tamanho++;
        return(true);
    }

    //lista não ordenda ou chave maior que o fim -> inserir fim 
    if (!lista->ordenada || item_get_chave(lista->fim->item) < item_get_chave(item)) {
        lista->fim->prox = no;
        lista->fim = no;
        lista->tamanho++;

        return(true);
    }

    //caso especial: a chave é menor que lista->inicio (inserir no inicio)
    if (item_get_chave(item) < item_get_chave(lista->inicio->item)) {
        no->prox = lista->inicio;
        lista->inicio = no;
        lista->tamanho++;

        return(true);
    }

    //chegou aqui: lista ordenada
    NO* aux = lista->inicio;

    while (aux != NULL && aux->prox != NULL) {
        if (item_get_chave(aux->prox->item) > item_get_chave(item)) {
            //inserir na posicao aux->prox
            no->prox = aux->prox;
            aux->prox = no;
            lista->tamanho++;

            return(true);
        }

        aux = aux->prox;
    }
    return(false);
}

ITEM* lista_remover(LISTA* lista, int chave) {
    
}

int lista_tamanho(LISTA* lista) {
    if (lista == NULL) {
        return(-1);
    }

    return(lista->tamanho);
}

bool lista_vazia(LISTA* lista) {
    if (lista == NULL) {
        return(false);
    }

    return(lista->tamanho == 0);
}

bool lista_cheia(LISTA* lista) {
    if (lista == NULL) {
        return(true);
    }

    NO* aux = (NO*) malloc(sizeof(NO));
    if (aux == NULL) {
        //sem memoria pra alocar -> cheia
        return(true);
    }

    free(aux);
    return(false);
}

void lista_imprimir(LISTA* lista) {
    if (lista == NULL) {
        return;
    }

    NO* aux = lista->inicio;
    while (aux != NULL) {
        printf("%d\n", item_get_chave(aux->item));
        aux = aux->prox;
    }
}

void lista_inverter(LISTA* lista) {
    if (lista_vazia(lista)) {
        return;
    }

    NO* aux = NULL;
    NO* final = lista->inicio;

    while (lista->inicio != lista->fim) {
        aux = lista->inicio;
        lista->inicio = aux->prox;

        aux->prox = lista->fim->prox;
        lista->fim->prox = aux;
    }

    lista->fim = final;
}