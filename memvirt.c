#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memvirt.h"
#include "cicQueue.c"
#include "list.c"

typedef struct result result_t;
typedef unsigned short int small_t;

typedef struct commnad_ {
  int pid;
  int pageNum;
  char control; //
}command_t;

typedef struct process_ {
  cicQueue_t *queue;
  uint32_t workingSet;
}process_t;

static result_t *initResult(int);
// void destryResult(struct result**);

static process_t *initProcess(int);
static void destryProcess(process_t **);

static command_t getCommand(FILE *);

struct result * memvirt(int num_procs, uint32_t num_frames, char * filename, uint32_t interval) {
  result_t *ret;
  FILE *file;
  command_t cmd;
  process_t **process; // List of pointers
  list_t *wsList;
  (void)wsList;
  ret = initResult(num_procs);
  file = fopen(filename, "r");
  assert(file);
  process = malloc(sizeof(process_t *) * num_procs);
  assert(process);
  for (small_t i = 0; i < num_procs; ++i) {
    process[i] = initProcess((int) (num_frames/num_procs));
  }

  uint32_t i = 0;
  int r;
  while (!feof(file)) {
    cmd = getCommand(file);
    if (cmd.control > 0) { // Check if read was sucessfull
      if (i==interval) {
        resizeQueue(&process[cmd.pid]->queue, (int) (num_frames/num_procs));
        i = 0;
      }
      r = accessPage(process[cmd.pid]->queue, cmd.pageNum);
      (*ret).refs[cmd.pid]++;
      if (r) {
        (*ret).pfs[cmd.pid]++;
      }
      i++;
    }
  }

  for (small_t i = 0; i < num_procs; ++i) {
    destryProcess(&process[i]);
  }
  free(process);
  fclose(file);
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

static void destryProcess(process_t **p) {
  destryQueue(&(*p)->queue);
  free(*p);
  return;
}

static process_t *initProcess(int ws) {
  process_t *ret;
  ret = malloc(sizeof(process_t));
  assert(ret);
  (*ret).queue = initQueue(ws);
  (*ret).workingSet = ws;
  return ret;
}

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
  for (small_t i = 0; i < qntProc; ++i) {
    (*ret).refs[i] = 0;
    (*ret).pfs[i] = 0;
    (*ret).pf_rate[i] = 0;
  }
  (*ret).avg_ws = 0;
  (*ret).total_pf_rate = 0.0;

  return ret;
}
