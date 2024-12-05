#include "set.h"
#include <stdio.h>

int main(void) {
    int ed, tam_set, tam_set2, chave; // 0 para avl, 1 para rubro-negro
    scanf("%d", &ed);

    SET *set = set_criar(ed);
    SET *set2 = set_criar(ed);

    scanf("%d %d", &tam_set, &tam_set2);

    for(int i = 0; i < tam_set; i++){
        scanf("%d", &chave);
        set_inserir(set, chave);
    }

    for(int i = 0; i < tam_set2; i++){
        scanf("%d", &chave);
        set_inserir(set2, chave);
    }

    SET *set3 = set_interseccao(set, set2);

    set_remover(set3, 1);

    set_imprimir(set3);

    set_apagar(&set);

    return(0);
}