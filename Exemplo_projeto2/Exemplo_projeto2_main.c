#include "set.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  SET *A, *B;
  int n_a, n_b, x;
  int op;
  int tipo;

  scanf("%d", &tipo); /*0 -> ED1, 1 -> ED2*/

  A = set_criar(tipo);
  B = set_criar(tipo);

  scanf("%d %d", &n_a, &n_b);

  for(int i=0; i<n_a; i++){
    scanf("%d",&x);
    set_inserir(A, x);

  }

  for(int i=0; i<n_b; i++){
    scanf("%d",&x);
    set_inserir(B, x);

  }

  scanf("%d", &op);

  switch(op){
    case 1: {
              //pertence
              ...
              break;
    }
    case 2: {
              //união
              ...
              break;
    }
    case 3: {
              //intersecção
              ...
              set_imprimir(???);
              break;
    }
    case 4: {
              //remoção
              ...
              set_imprimir(???); 
    }
  }

  ....
    
  return 0;
}
