/**
 * Projeto 2 - Estruturas de Dados e Algoritmos
 *
 * @author Augusto Fernandes Ildefonso
 * @author Renan Trofino
 */
#include "set.h"
#include "item.h"
#include <stdio.h>

/**
 * Função principal
 *
 * @return Retorna 0 se for bem sucedido
 */
int main(void) {
    int ed;  // 0 para avl, 1 para rubro-negro
    int tam_set, tam_set2, chave;
    int op, set_alvo;

    scanf("%d", &ed); // Recebe o tipo de árvore que o set vai utilizar

    SET *A = set_criar(ed);
    SET *B = set_criar(ed);
    SET *C = NULL;

    scanf("%d %d", &tam_set, &tam_set2); // Lê o tamanho de cada set

    // Recebe valores do set 1
    for(int i = 0; i < tam_set; i++){
        scanf("%d", &chave);
        set_inserir(A, item_criar(chave, NULL));
    }

    // Recebe valoes do set 2
    for(int i = 0; i < tam_set2; i++){
        scanf("%d", &chave);
        set_inserir(B, item_criar(chave, NULL));
    }

    // Lê a operação que será feita
    scanf("%d", &op);

    switch(op) {
        case 1:
            //pertence
            scanf("%d %d", &set_alvo, &chave);
            if (set_alvo == 1) {
                printf("%s\n", set_pertence(A, chave) ? "Pertence!" : "Não Pertence!");
            } else {
                printf("%s\n", set_pertence(B, chave) ? "Pertence!" : "Não Pertence!");
            }
            break;
        case 2:
            //uniao
            C = set_uniao(A, B);
            set_imprimir(C);
            break;
        case 3:
            //interseccao
            C = set_interseccao(A, B);
            set_imprimir(C);
            break;
        case 4:
            scanf("%d %d", &set_alvo, &chave);
            if (set_alvo == 1) {
                printf("%s\n", set_remover(A, chave) ? "Removido!" : "Não Removido!");
            } else {
                printf("%s\n", set_remover(B, chave) ? "Removido!" : "Não Removido!");
            }
            break;
    }

    set_apagar(&A);
    set_apagar(&B);

    //Erro ao apagar set C
    //set_apagar(&C);

    return(0);
}