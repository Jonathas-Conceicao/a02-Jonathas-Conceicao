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

void insertList(list_t *list, uint32_t x) {
  if ((*list).size + 1 >= (*list).size) assert(1);
  (*list).array[(*list).nEle] = x;
  (*list).nEle += 1;
  return;
}
