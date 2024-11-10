#ifndef _ARVORE_BINARIA_H
  #define _ARVORE_BINARIA_H
 
    #include  "item.h"
    #define FILHO_ESQ 0
    #define FILHO_DIR 1

    typedef struct arv_bin AB;
    
    AB *ab_criar(void);
	  void ab_apagar(AB **arvore);

    bool ab_inserir(AB *arvore, ITEM *item, int lado, int chave);
    void ab_imprimir(AB *arvore);
#endif
