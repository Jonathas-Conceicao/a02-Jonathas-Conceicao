#include "memvirt.h"
#include "simpletest.h"
#include <stdio.h>


#define WS(a, b, c) \
if (a){ \
	isEqual(a->avg_ws, b, c); \
} \
else isNotNull(a, c);

#define PFRATE(a, b, c) \
if (a){ \
	isNear(a->total_pf_rate, b, c); \
} \
else isNotNull(a, c);



void test_small(){
	struct result * res;

	WHEN("Tenho apenas um processo, um frame e uma página");
	THEN("Somente o primeiro acesso é pf");

	res = memvirt(1,1,"small.txt",1);
	WS(res, 1, 1);
	PFRATE(res, 0.1, 1);
	if(res)
		free(res);
}

void test_small2(){
	struct result * res;

	WHEN("Tenho apenas um processo, um frame e 10 páginas sendo acessadas em sequencia");
	THEN("Todo acesso é pf");

	res = memvirt(1,1,"small2.txt",1);
	WS(res, 1, 1);
	PFRATE(res, 1, 1);
	if(res)
		free(res);
}

void test_small3(){
	struct result * res;

	WHEN("Tenho apenas um processo, dois frames e 5 páginas sendo acessadas com repetições");
	THEN("Metade dos acessos é pf");

	res = memvirt(1,2,"small3.txt",5);
	WS(res, 2, 1);
	PFRATE(res, .3, 1);
	if(res)
		free(res);
}

void test_2_procs(){
  struct result * res;

  WHEN("Tenho dois processos, cada um com duas páginas, quatro frames e equilíbrio de acessos");
  THEN("Espero ter só pf compulsórios");

  res = memvirt(2,4,"twoprocs.txt",4);
  WS(res, 2, 2);
  PFRATE(res, .75, 1);
  if(res)
    free(res);
}

int main(int argc, char ** argv){
  (void)argc;
  (void)argv;

	test_small();
	test_small2();
	test_small3();
	test_2_procs();

	printf("Grading \n");
	GRADEME();

	if (grade==maxgrade)
		return 0;
	else return grade;



}
