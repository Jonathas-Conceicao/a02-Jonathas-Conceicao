#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <assert.h>

#ifndef _SMALL_T_
#define _SMALL_T_
typedef unsigned short int small_t;
#endif

typedef struct elementQueue_ {
  signed int pageNum;
  char refBit; //
}elementQueue_t;

typedef struct nodeQueue_ {
  elementQueue_t element;
  struct nodeQueue_ *next;
  struct nodeQueue_ *prev;
}nodeQueue_t;

typedef struct queue_ {
  nodeQueue_t *head;
  nodeQueue_t *arrow;
  small_t nEle;
  small_t maxSize;
}queue_t;

queue_t *initQueue(int);
void destryQueue(queue_t **);
nodeQueue_t *initNodeQueue(int, nodeQueue_t*, nodeQueue_t*);

void setArrowPosQueue(queue_t *);

void resizeQueue(queue_t*, signed int);
static void removeNElements(queue_t*, unsigned int);
static void increaceInNElements(queue_t*, unsigned int);

void insertPageQueue(queue_t *, int);
void removePageQueue(queue_t *);
int findPageQueue(queue_t *, int);

int accessPageQueue(queue_t *, int);

#endif

queue_t *initQueue(int inicialSize) {
  queue_t *ret;
  ret = malloc(sizeof(queue_t));
  assert(ret);
  ret->head = NULL;
  ret->arrow = NULL;
  ret->nEle = 0;
  ret->maxSize = inicialSize;
  return ret;
}
void destryQueue(queue_t **ppQueue) {
  nodeQueue_t *aux;
  (*ppQueue)->head->prev->next = NULL; //Makes the last element point to NULL
  while (((*ppQueue)->head) != NULL) {
    aux = (*ppQueue)->head;
    (*ppQueue)->head = (*ppQueue)->head->next;
    free(aux);
  }
  free(*ppQueue);
  return;
}
nodeQueue_t *initNodeQueue(int page, nodeQueue_t *n, nodeQueue_t *p) {
  nodeQueue_t *ret;
  ret = malloc(sizeof(nodeQueue_t));
  assert(ret);
  ret->element.pageNum = page;
  ret->element.refBit = 1;
  if (n) {
    ret->next = n;
  } else {
    ret->next = ret;
  }
  if (p) {
    ret->prev = p;
  } else {
    ret->prev = ret;
  }
  return ret;
}

void setArrowPosQueue(queue_t *pQ) {
  if (pQ->head != NULL) {
    while (pQ->arrow->element.refBit != 0) {
      pQ->arrow->element.refBit = 0;
      pQ->arrow = pQ->arrow->next;
    }
  }
  return;
}

void resizeQueue(queue_t *pQ, signed int size) {
  if (size < 0) {
    removeNElements(pQ, abs(size));
  } else {
    increaceInNElements(pQ, abs(size));
  }
  return;
}
static void increaceInNElements(queue_t *pQ, unsigned int n) {
  pQ->maxSize += n;
  return;
}
static void removeNElements(queue_t *pQ, unsigned int n) {
  for (small_t i = pQ->maxSize; i > (pQ->maxSize - n); --i) {
    setArrowPosQueue(pQ);
    removePageQueue(pQ);
  }
  return;
}

void insertPageQueue(queue_t *pQ, int page) {
  if (pQ->head != NULL) {
    if (pQ->nEle < pQ->maxSize) {
      nodeQueue_t *newNode;
      newNode = initNodeQueue(page, pQ->arrow, pQ->arrow->prev);
      pQ->arrow->prev->next = newNode;
      pQ->arrow->prev = newNode;
      pQ->nEle++;
    } else {
      setArrowPosQueue(pQ);
      pQ->arrow->element.pageNum = page;
      pQ->arrow->element.refBit = 1;
    }
  } else {
    nodeQueue_t *newNode;
    newNode = initNodeQueue(page, NULL, NULL);
    pQ->arrow = newNode;
    pQ->head = newNode;
    pQ->nEle++;
  }
  return;
}
void removePageQueue(queue_t *pQ) {
  assert(pQ->head == NULL);
  assert(pQ->nEle > 1);
  nodeQueue_t *toFree;
  toFree = pQ->arrow;
  pQ->arrow->prev->next = pQ->arrow->next;
  pQ->arrow->next->prev = pQ->arrow->prev;
  pQ->arrow = pQ->arrow->next;
  free(toFree);
  pQ->nEle--;
  pQ->maxSize--;
}
int accessPageQueue(queue_t *pQ, int page) {
  nodeQueue_t *aux;
  if (pQ->head == NULL) return 1;
  aux = pQ->head;
  do {
    if (aux->element.pageNum == page) {
      aux->element.refBit = 1;
      return 0; // If page was found
    }
    aux = aux->next;
  } while(aux != pQ->head && aux != NULL);
  return 1;
}
