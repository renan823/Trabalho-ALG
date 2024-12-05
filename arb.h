#ifndef ARB_H
    #define ARB_H

    #include <stdbool.h>

    typedef struct arb ARB;

    ARB *arb_criar(void);
    void arb_apagar(ARB **arvore);

    bool arb_inserir(ARB *arvore, int chave);
    int arb_remover(ARB *arvore, int chave);
    int arb_buscar(ARB *arvore, int chave);

    ARB *arb_unir(ARB *a1, ARB *a2);
    ARB *arb_intersectar(ARB *a1, ARB *a2);

    void arb_imprimir(ARB *arvore);
#endif 