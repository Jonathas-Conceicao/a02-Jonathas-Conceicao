#include <stdlib.h>
#include <assert.h>

#ifndef _LIST_H_
#define _LIST_H_

#ifndef _SMALL_T_
#define _SMALL_T_
typedef unsigned short int small_t;
#endif

typedef struct list_ {
  uint32_t *list; // Array
  small_t nEle;
  small_t size;
}list_t;

#endif

list_t initList(small_t size) {
  list_t *ret;
  ret = malloc(sizeof(list_t));
  (*ret).list = malloc(sizeof(uint32_t) * size);
  (*ret).size = size;
  return (*ret);
}
