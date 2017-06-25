#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memvirt.h"

typedef struct result result_t;
typedef unsigned short int small_t;

typedef struct commnad_ {
  int pid;
  int pageNum;
  char control; //
}command_t;

typedef struct element_ {
  int pageNum;
  char bitRef; //
}element_t;

typedef struct cicQueue_ {
  element_t *list;
  small_t arrow;
  small_t inPos;
  small_t size;
}cicQueue_t;

static result_t *initResult(int);
// void destryResult(struct result**);

static cicQueue_t *initQueue(int);
static void destryQueue(cicQueue_t **);

static void insertPage(cicQueue_t *q, int page);
static int findPage(cicQueue_t *, int);
static void setArrowPos(cicQueue_t *);

static command_t getCommand(FILE *);
static void accessPage(cicQueue_t *, int);

struct result * memvirt(int num_procs, uint32_t num_frames, char * filename, uint32_t interval) {
  (void)interval;
  result_t *ret;
  FILE *file;
  command_t cmd;
  cicQueue_t **queues;

  ret = initResult(num_procs);
  file = fopen(filename, "r");
  assert(file);
  queues = malloc(sizeof(cicQueue_t *) * num_procs);
  assert(queues);
  for (small_t i = 0; i < num_procs; i++) {
    queues[i] = initQueue(num_frames/num_procs);
  }

  while (!feof(file)) {
    cmd = getCommand(file);
    if (cmd.control > 0) { // Check if read was sucessfull
      // printf("PID:%i -- Page:%i\n", cmd.pid, cmd.pageNum);
      accessPage(queues[cmd.pid], cmd.pageNum);
    }
  }

  for (small_t i = 0; i < num_procs; i++) {
    destryQueue(&queues[i]);
  }
  free(queues);
  fclose(file);
  return ret;
}

static void accessPage(cicQueue_t *q, int page) {
  int aux;
  aux = findPage(q, page);
  if (aux > 0) { // Page was in the list.
    (*q).list[aux].bitRef = 1; // Reset the reference bit.
    return; // Nothing more to do.
  }
  insertPage(q, page);
  return;
}

static void insertPage(cicQueue_t *q, int page) {
  setArrowPos(q); // Look for a propper slot.
  (*q).list[(*q).arrow].pageNum = page;
  (*q).list[(*q).arrow].bitRef = 1;
  (*q).arrow++;
  if ((*q).arrow >= (*q).size)
    (*q).arrow = 0;
  return;
}

static void setArrowPos(cicQueue_t *q) { // Looks for a propper slot.
  while((*q).list[(*q).arrow].bitRef != 0) {
    (*q).list[(*q).arrow].bitRef = 0; // Gives a second chance.
    (*q).arrow++; // Goes to next position,
    if ((*q).arrow >= (*q).size)
      (*q).arrow = 0;
  }
  return;
}

static int findPage(cicQueue_t *q, int page) {
  for (small_t i = 0; i < (*q).size; i++) {
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

static cicQueue_t *initQueue(int size) {
  cicQueue_t *ret;
  ret = malloc(sizeof(cicQueue_t));
  assert(ret);
  (*ret).list = malloc(sizeof(element_t) * size);
  assert((*ret).list);
  for (small_t i = 0; i < size; i++) {
    (*ret).list[i].pageNum = 0;
    (*ret).list[i].bitRef = 0;
  }
  (*ret).inPos = 0;
  (*ret).arrow = 0;
  (*ret).size = size;
  return ret;
}

static command_t getCommand(FILE *f) {
  command_t ret;
  ret.control = fscanf(f, "%i %i%*c", &ret.pid, &ret.pageNum);
  return ret;
}

// extern void destryResult(result_t** pR) {
//   free((*pR)->refs);
//   free((*pR)->pfs);
//   free((*pR)->pf_rate);
//   free(*pR);
// }

static result_t *initResult(int qntProc) {
  result_t *ret;
  ret = malloc(sizeof(result_t));
  assert(ret);

  (*ret).refs = malloc(sizeof(uint32_t) * qntProc);
  assert((*ret).refs);
  (*ret).pfs = malloc(sizeof(uint32_t) * qntProc);
  assert((*ret).pfs);
  (*ret).pf_rate = malloc(sizeof(float) * qntProc);
  assert((*ret).pf_rate);
  for (small_t i = 0; i < qntProc; i++) {
    (*ret).refs[i] = 0;
    (*ret).pfs[i] = 0;
    (*ret).pf_rate[i] = 0;
  }
  (*ret).avg_ws = 0;
  (*ret).total_pf_rate = 0.0;

  return ret;
}
