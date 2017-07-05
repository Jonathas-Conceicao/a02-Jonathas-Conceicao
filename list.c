#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#ifndef _SMALL_T_
#define _SMALL_T_
typedef unsigned short int small_t;
#endif

typedef struct nodeList_ {
  uint32_t element;
  struct nodeList_ *next;
}nodeList_t;

typedef struct list_ {
  nodeList_t *head;
  small_t size;
}list_t;

list_t *initList();
void destryList(list_t **);
nodeList_t *initNodeList(uint32_t);

small_t getNElemList(list_t);
small_t getMaxSizeList(list_t);

int insertList(list_t *, uint32_t);
uint32_t getElementAtList(list_t, unsigned int);

#endif


list_t *initList() {
  list_t *ret;
  ret = malloc(sizeof(list_t));
  assert(ret);
  (*ret).head = NULL;
  (*ret).size = 0;
  return ret;
}

void destryList(list_t **pList) {
  nodeList_t *aux;
  while (((*pList)->head) != NULL) {
    aux = (*pList)->head;
    (*pList)->head = (*pList)->head->next;
    free(aux);
  }
  free(*pList);
}

small_t getNElemList(list_t l) {
  return l.size;
}

nodeList_t *initNodeList(uint32_t val) {
  nodeList_t *ret;
  ret = malloc(sizeof(nodeList_t));
  assert(ret);
  ret->element = val;
  ret->next = NULL;
  return ret;
}

int insertList(list_t *pL, uint32_t val) {
  if (pL->head != NULL) {
    nodeList_t *aux;
    aux = pL->head;
    while (aux->next != NULL && aux->element != val) {
      aux = aux->next;
    }
    if (aux->element == val) return 0;
    nodeList_t *newNode;
    newNode = initNodeList(val);
    aux->next = newNode;
  } else {
    nodeList_t *newNode;
    newNode = initNodeList(val);
    pL->head = newNode;
  }
  return 1;
}

uint32_t getElementAtList(list_t l, unsigned int n) {
  nodeList_t *aux;
  aux = l.head;
  for (small_t i = 0; i < n; ++i) {
    aux = aux->next;
  }
  return aux->element;
}
