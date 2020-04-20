#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_sem.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};
struct Well* well;

struct Well {
  uthread_sem_t mutex;
  uthread_sem_t max;
  uthread_sem_t bigQueue;
  uthread_sem_t littleQueue;
  uthread_sem_t priorityQueue;
  int endianness;
  int occupancy;
};

struct Well* createWell() {
  struct Well* well = malloc (sizeof (struct Well));
  well->max = uthread_sem_create(MAX_OCCUPANCY);
  well->mutex = uthread_sem_create(1);
  well->bigQueue = uthread_sem_create(0);
  well->littleQueue = uthread_sem_create(0);
  well->priorityQueue = uthread_sem_create(0);
  well->endianness = 0;                               // 0 for neither, 1 for little, 2 for big.
  well->occupancy = 0;
  return well;
}

struct Person {
  enum Endianness endianness;
  int lineStart;                 
  int waitTime;                
};

struct Person* createPerson(int random) {
  struct Person* p = malloc(sizeof(struct Person));
  p->endianness = oppositeEnd[random];
  p->lineStart = 0;
  return p;
}


#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t   waitingHistogramMutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_sem_wait(waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal(waitingHistogramMutex);
}

// wait if max occupancy, then check if endianness is the same
// then update the state of the program
void enterwell (struct Person* p) {
  uthread_sem_wait(well->mutex);
  p->lineStart = entryTicker;
  if (well->endianness != 0) {
    printf("endian %d well endian %d\n", p->endianness, well->endianness);
    (p->endianness != (well->endianness - 1)) // wait if endianness is different
      ? (uthread_sem_wait(well->bigQueue)) : (uthread_sem_wait (well->littleQueue));
  }
  printf("stuck at occupancy wait\n");
  uthread_sem_wait(well->max); // wait on occupancy
  printf("endian %d stuck at endianness wait\n", p->endianness);
  printf("stuck at mutex wait\n");

  assert(well->occupancy <= 3);
  well->occupancy++;
  printf("occupancy %d\n", well->occupancy);
  if (well->endianness = 0)
    well->endianness = ((int) p->endianness) + 1;
  printf("endianness %d\n", p->endianness);
  printf("record well endian %d occup %d\n", well->endianness, well->occupancy);
  occupancyHistogram[well->endianness - 1][well->occupancy]++;
  p->waitTime = entryTicker - p->lineStart;
  recordWaitingTime(p->waitTime);
  entryTicker++;
  printf("about to signal\n");
  uthread_sem_signal(well->mutex);
}

void leavewell(struct Person* p) {
  uthread_sem_wait(well->mutex);
  printf("about to decrement\n");
  well->occupancy--;
  //assert(well->occupancy >= 0);
  if (well->occupancy = 0)
    well->endianness = 0;
  (well->endianness - 1) ? uthread_sem_signal(well->bigQueue) : uthread_sem_signal(well->littleQueue);
  uthread_sem_signal(well->max);
  uthread_sem_signal(well->mutex);
}

void* person(void* pv) {
  struct Person * p = pv;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    enterwell(p);
    
    for (int j = 0; j < NUM_ITERATIONS; j++) 
      uthread_yield();
    
    leavewell(p);
    
    for (int k = 0; k < NUM_ITERATIONS; k++) 
      uthread_yield();
    
  }
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (1);
  well = createWell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogramMutex = uthread_sem_create (1);

  for (int i = 0; i < NUM_PEOPLE; i++) {
    int n = random() % 2;
    struct Person* p = createPerson(n);
    pt[i] = uthread_create(person, p);
  }

  for (int i = 0; i < NUM_PEOPLE; i++)
    uthread_join(pt[i],NULL);

  printf ("Times with 1 little endian %d\n", occupancyHistogram [LITTLE]   [1]);
  printf ("Times with 2 little endian %d\n", occupancyHistogram [LITTLE]   [2]);
  printf ("Times with 3 little endian %d\n", occupancyHistogram [LITTLE]   [3]);
  printf ("Times with 1 big endian    %d\n", occupancyHistogram [BIG] [1]);
  printf ("Times with 2 big endian    %d\n", occupancyHistogram [BIG] [2]);
  printf ("Times with 3 big endian    %d\n", occupancyHistogram [BIG] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
  if (waitingHistogram [i])
  printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
  printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
