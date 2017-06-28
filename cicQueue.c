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
  signed int pageNum;
  char bitRef; //
}element_t;

typedef struct cicQueue_ {
  element_t *list;
  small_t arrow;
  small_t nEle;
  small_t size;
}cicQueue_t;

cicQueue_t *initQueue(int);
void destryQueue(cicQueue_t **);
void resizeQueue(cicQueue_t**, signed int);
void removeNElements(cicQueue_t**, unsigned int);
void increaceInNElements(cicQueue_t**, unsigned int);

void insertPage(cicQueue_t *q, int page);
int findPage(cicQueue_t *, int);
void setArrowPos(cicQueue_t *);

int accessPage(cicQueue_t *, int);

#endif

int accessPage(cicQueue_t *q, int page) {
  int aux;
  aux = findPage(q, page);
  if (aux >= 0) { // Page was in the list.
    (*q).list[aux].bitRef = 1; // Reset the reference bit.
    return 0; // Nothing more to do.
  }
  insertPage(q, page);
  return 1;
}

void insertPage(cicQueue_t *q, int page) {
  setArrowPos(q); // Look for a propper slot.
  (*q).list[(*q).arrow].pageNum = page;
  (*q).list[(*q).arrow].bitRef = 1;
  (*q).arrow++;
  (*q).nEle++;
  if ((*q).arrow >= (*q).size)
    (*q).arrow = 0;
  return;
}

void setArrowPos(cicQueue_t *q) { // Looks for a propper slot.
  while((*q).list[(*q).arrow].bitRef != 0) {
    (*q).list[(*q).arrow].bitRef = 0; // Gives a second chance.
    (*q).arrow++; // Goes to next position,
    if ((*q).arrow >= (*q).size) (*q).arrow = 0;
  }
  if ((*q).list[(*q).arrow].pageNum != -1) (*q).nEle--; // If page was not empty we are removing an element
  return;
}

int findPage(cicQueue_t *q, int page) {
  for (small_t i = 0; i < (*q).size; ++i) {
    if ((*q).list[i].pageNum == page)
      return i;
  }
  return -1;
}

void destryQueue(cicQueue_t **pQ) {
  free((*pQ)->list);
  free(*pQ);
  return;
}

void removeNElements(cicQueue_t **q, unsigned int numOfElements) {
  assert(*q);
  if (numOfElements == 0) return;
  setArrowPos(*q);
  for (small_t i = (*q)->arrow; i < ((*q)->size -1); ++i) { // Pull all elements to the end one position behind
    (*q)->list[i].pageNum = (*q)->list[i+1].pageNum;
    (*q)->list[i].bitRef = (*q)->list[i+1].bitRef;
  }
  element_t *aux;
  aux = realloc((*q)->list, sizeof(element_t) * ((*q)->size - 1)); // Resize's list o size-1
  assert(aux);
  (*q)->list = aux;
  (*q)->size -=  1;
  removeNElements(q, (numOfElements - 1)); // Just because recursion is fun
}

void increaceInNElements(cicQueue_t **q, unsigned int numOfElements) {
  assert(*q);
  element_t *aux;
  aux = realloc((*q)->list, sizeof(element_t) * ((*q)->size + numOfElements));
  assert(aux);
  (*q)->list = aux;
  for (unsigned int i = ((*q)->size)-1; i < ((*q)->size + numOfElements); ++i) { // Inicializar novas posições
    (*q)->list[i].pageNum = -1; // Valor default para representar espaço vazio
    (*q)->list[i].bitRef = 0;
  }
  (*q)->size += numOfElements;
  (*q)->arrow = (*q)->size-1;
  return;
}

void resizeQueue(cicQueue_t **q, signed int resizeValue) {
  (resizeValue > 0) ? increaceInNElements(q, abs(resizeValue)) : removeNElements(q, abs(resizeValue));
  return;
}

cicQueue_t *initQueue(int size) {
  cicQueue_t *ret;
  ret = malloc(sizeof(cicQueue_t));
  assert(ret);
  (*ret).list = malloc(sizeof(element_t) * size);
  assert((*ret).list);
  for (small_t i = 0; i < size; ++i) {
    (*ret).list[i].pageNum = -1; // Valor default para representar espaço vazio
    (*ret).list[i].bitRef = 0;
  }
  (*ret).arrow = 0;
  (*ret).nEle = 0;
  (*ret).size = size;
  return ret;
}
