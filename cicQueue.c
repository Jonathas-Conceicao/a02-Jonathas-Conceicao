#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef _CICQUEUE_H_
#define _CICQUEUE_H_

#ifndef _SMALL_T_
#define _SMALL_T_
typedef unsigned short int small_t;
#endif

typedef struct element_ {
  int pageNum;
  char bitRef; //
}element_t;

typedef struct cicQueue_ {
  element_t *list;
  small_t arrow;
  small_t nEle;
  small_t size;
}cicQueue_t;

static cicQueue_t *initQueue(int);
static void destryQueue(cicQueue_t **);
static void resizeQueue(cicQueue_t**, int);

static void insertPage(cicQueue_t *q, int page);
static int findPage(cicQueue_t *, int);
static void setArrowPos(cicQueue_t *);

static int accessPage(cicQueue_t *, int);

#endif

static int accessPage(cicQueue_t *q, int page) {
  int aux;
  aux = findPage(q, page);
  if (aux >= 0) { // Page was in the list.
    (*q).list[aux].bitRef = 1; // Reset the reference bit.
    return 0; // Nothing more to do.
  }
  insertPage(q, page);
  return 1;
}

static void insertPage(cicQueue_t *q, int page) {
  setArrowPos(q); // Look for a propper slot.
  (*q).list[(*q).arrow].pageNum = page;
  (*q).list[(*q).arrow].bitRef = 1;
  (*q).arrow++;
  (*q).nEle++;
  if ((*q).arrow >= (*q).size)
    (*q).arrow = 0;
  return;
}

static void setArrowPos(cicQueue_t *q) { // Looks for a propper slot.
  while((*q).list[(*q).arrow].bitRef != 0) {
    (*q).list[(*q).arrow].bitRef = 0; // Gives a second chance.
    (*q).arrow++; // Goes to next position,
    if ((*q).arrow >= (*q).size) (*q).arrow = 0;
  }
  if ((*q).list[(*q).arrow].pageNum != 0) (*q).nEle--; // If page was not empty we are removing an element
  return;
}

static int findPage(cicQueue_t *q, int page) {
  for (small_t i = 0; i < (*q).size; ++i) {
    if ((*q).list[i].pageNum == page)
      return i;
  }
  return -1;
}

static void destryQueue(cicQueue_t **pQ) {
  free((*pQ)->list);
  free(*pQ);
  return;
}

static void resizeQueue(cicQueue_t **q, int newSize) {
  assert(*q);
  if (newSize == (*q)->size)
    return;
  (*q)->list = realloc((*q)->list, sizeof(element_t) * newSize);
  for (int i = ((*q)->size)-1; i < newSize; ++i) { // Inicializar novas posições
    (*q)->list[i].pageNum = 0;
    (*q)->list[i].bitRef = 0;
  }
  if (newSize < (*q)->size)
    (*q)->arrow = newSize-1;
  (*q)->size = newSize;
  return;
}

static cicQueue_t *initQueue(int size) {
  cicQueue_t *ret;
  ret = malloc(sizeof(cicQueue_t));
  assert(ret);
  (*ret).list = malloc(sizeof(element_t) * size);
  assert((*ret).list);
  for (small_t i = 0; i < size; ++i) {
    (*ret).list[i].pageNum = 0;
    (*ret).list[i].bitRef = 0;
  }
  (*ret).arrow = 0;
  (*ret).nEle = 0;
  (*ret).size = size;
  return ret;
}
