#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memvirt.h"
#include "cicQueue.c" // Tu sabes porque isso ta feio assim.
#include "list.c" // Tu sabes porque isso ta feio assim.

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
  list_t *wsList;

  wsList = initList(num_procs);
  ret = initResult(num_procs);
  file = fopen(filename, "r");
  assert(file);
  process = malloc(sizeof(process_t *) * num_procs);
  assert(process);
  for (small_t i = 0; i < num_procs; ++i) {
    process[i] = initProcess((int) (num_frames/num_procs));
    process[i]->workingSet = (int) (num_frames/num_procs); // Initinal value of working sets
    insertList(wsList, (int) (num_frames/num_procs));
  }
  interval = 0;
  uint32_t i = 0;
  int r;
  while (!feof(file)) {
    cmd = getCommand(file);
    if (cmd.control > 0) { // Check if read was sucessfull
      if (i==interval) {
        resizeQueue(&process[cmd.pid]->queue, 0);
        i = 0;
        for (small_t i = 0; i < num_procs; ++i) { // Updates number of frames for each process
        }
      }
      r = accessPage(process[cmd.pid]->queue, cmd.pageNum);
      (*ret).refs[cmd.pid]++;
      if (r) {
        (*ret).pfs[cmd.pid]++;
      }
      i++;
    }
  }

  for (small_t i = 0; i < getSizeList(*wsList); ++i) {
    (*ret).avg_ws += getElementAtList(*wsList, i);
  }
  (*ret).avg_ws /= getSizeList(*wsList);

  for (small_t i = 0; i < num_procs; ++i) {
    (*ret).pf_rate[i] = ((float) (*ret).pfs[i]/(*ret).refs[i]) * 100;
    destryProcess(&process[i]);
  }
  destryList(&wsList);
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
  process_t *ret;
  ret = malloc(sizeof(process_t));
  assert(ret);
  (*ret).queue = initQueue(ws);
  (*ret).workingSet = ws;
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
