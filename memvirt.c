#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memvirt.h"
#include "queue.c" // Tu sabes porque isso ta feio assim.
#include "list.c" // Tu sabes porque isso ta feio assim.

typedef struct result result_t;

typedef struct commnad_ {
  int pid;
  int pageNum;
  char control; //
}command_t;

typedef struct process_ {
  queue_t *queue;
  uint32_t opExecuted;
  uint32_t workingSet;
}process_t;

result_t *initResult(int);
// void destryResult(struct result**);

process_t *initProcess(int);
void destryProcess(process_t **);

command_t getCommand(FILE *);

struct result * memvirt(int num_procs, uint32_t num_frames, char * filename, uint32_t interval) {
  result_t *ret;
  FILE *file;
  command_t cmd;
  process_t **process; // List of pointers
  small_t r;
  process = malloc(sizeof(process_t *) * num_procs);
  assert(process);
  ret = initResult(num_procs);
  assert(ret);
  for (small_t i = 0; i < num_procs; ++i) {
    process[i] = initProcess(num_frames/num_procs);
  }

  (void)interval;
  file = fopen(filename, "r");
  assert(file);
  while (!feof(file)) {
    cmd = getCommand(file);
    if (cmd.control > 0) { // Check if read was sucessfull
      r = accessPageQueue(process[cmd.pid]->queue, cmd.pageNum); // Returns 0 if read was sucessfull, 1 if it's a page fault
      (*ret).refs[cmd.pid]++;
      if (r) {
        (*ret).pfs[cmd.pid]++;
        insertPageQueue(process[cmd.pid]->queue, cmd.pageNum);
      }
    }
  }

  for (small_t i = 0; i < num_procs; ++i) {
    destryProcess(&process[i]);
  }
  free(process);
  fclose(file);
  return ret;
}

command_t getCommand(FILE *f) {
  command_t ret;
  ret.control = fscanf(f, "%i %i%*c", &ret.pid, &ret.pageNum);
  return ret;
}

// void destryResult(result_t** pR) {
//   free((*pR)->refs);
//   free((*pR)->pfs);
//   free((*pR)->pf_rate);
//   free(*pR);
// }

void destryProcess(process_t **p) {
  destryQueue(&(*p)->queue);
  free(*p);
  return;
}

process_t *initProcess(int ws) {
  if (ws < 1) ws = 1;
  process_t *ret;
  ret = malloc(sizeof(process_t));
  assert(ret);
  (*ret).queue = initQueue(ws);
  (*ret).workingSet = ws;
  (*ret).opExecuted = 0;
  return ret;
}

result_t *initResult(int qntProc) {
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
