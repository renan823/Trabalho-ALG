
#ifndef LISTA_H
	#define LISTA_H

	#include "item.h"
	#include <stdbool.h>

	typedef struct lista_ LISTA;

	LISTA* lista_criar(bool ordenada);
	void lista_apagar(LISTA** lista);

	bool lista_inserir(LISTA* lista, ITEM* item);
	ITEM* lista_remover(LISTA*, int chave);

	int lista_tamanho(LISTA* lista);
	bool lista_vazia(LISTA* lista);
	bool lista_cheia(LISTA* lista);

	void lista_imprimir(LISTA* lista);
	void lista_inverter(LISTA* lista);
#endif
