#include <stdlib.h>
#include <assert.h>

#ifndef _LIST_H_
#define _LIST_H_

#ifndef _SMALL_T_
#define _SMALL_T_
typedef unsigned short int small_t;
#endif

typedef struct list_ {
  uint32_t *array;
  small_t nEle;
  small_t size;
}list_t;

void destryList(list_t **);
list_t *initList(small_t);
void resize(list_t *);

small_t getSizeList(list_t);
small_t getMaxSizeList(list_t);

void insertList(list_t *, uint32_t);
uint32_t getElementAtList(list_t, int);
#endif

list_t *initList(small_t size) {
  list_t *ret;
  ret = malloc(sizeof(list_t));
  assert(ret);
  (*ret).array = malloc(sizeof(uint32_t) * size);
  assert((*ret).array);
  (*ret).size = size;
  (*ret).nEle = 0;
  return ret;
}

void destryList(list_t **pPL) {
  free((*pPL)->array);
  free((*pPL));
  return;
}

uint32_t getElementAtList(list_t l, int index) {
  assert(index < 0 ||
         l.size > index);
  return l.array[index];
}

void insertList(list_t *pL, uint32_t x) {
  if ((*pL).size + 1 >= (*pL).size) resize(pL);
  (*pL).array[(*pL).nEle] = x;
  (*pL).nEle += 1;
  return;
}

small_t getSizeList(list_t l) {
  return l.nEle;
}
small_t getMaxSizeList(list_t l) {
  return l.size;
}

void resize(list_t *pL) {
  assert(pL);
  uint32_t *aux;
  aux = realloc((*pL).array, sizeof(uint32_t) * ((*pL).size * 2));
  assert(aux);
  (*pL).array = aux;
  (*pL).size *= 2;
}
