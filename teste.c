#include <stdio.h>
#include "memvirt.h"
#include "simpletest.h"

void destryResult(struct result** pR) {
  free((*pR)->refs);
  free((*pR)->pfs);
  free((*pR)->pf_rate);
  free(*pR);
  return;
}

void test1(void) {
  WHEN("Teste simples com 12 operações.");
  IF("Três processos sem recálculo de working set acessando apenas duas páginas, mas com 9 frames no total.");

  struct result *ret;
  ret = memvirt(3, 9, "t01.txt", 12);

  THEN("Processo 0 deve ter 5 acessos.");
  isEqual(ret->refs[0], 5, 1);
  THEN("Processo 1 deve ter 3 acessos.");
  isEqual(ret->refs[1], 3, 1);
  THEN("Processo 2 deve ter 4 acessos.");
  isEqual(ret->refs[2], 4, 1);
  THEN("Todos os processos devem ter 2 page faults.");
  isEqual(ret->pfs[0], 2, 1);
  isNear(ret->pf_rate[0], 0.400, 1);
  isEqual(ret->pfs[1], 2, 1);
  isNear(ret->pf_rate[1], 0.666, 1);
  isEqual(ret->pfs[2], 2, 1);
  isNear(ret->pf_rate[2], 0.500, 1);

  THEN("O working set médio deve ser 3.");
  isEqual(ret->avg_ws, 3, 1);
  THEN("O total de page fault deve ser 50%.");
  isNear(ret->total_pf_rate, 0.50, 1);

  destryResult(&ret);
  return;
}

int main(int argc, char const *argv[]) {
  (void)argc; // Essa forma não bagunça o hightlight do meu editor.
  (void)argv;
  DESCRIBE("Testes do simulador de gerênciamento de meória virtual");
  test1();
  GRADEME();
  return 0;
}
