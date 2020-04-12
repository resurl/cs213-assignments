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
#define NUM_ITERATIONS     1
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};
struct Well* well;

struct Well {
  uthread_sem_t mutex;
  int impatient_big_size;
  int impatient_little_size;
  int endianness;
  int occupancy;
  uthread_sem_t littleWaiting;
  uthread_sem_t bigWaiting;
  uthread_sem_t impatient_big;
  uthread_sem_t impatient_little;
  uthread_sem_t max;
};

struct Well* createwell() {
  struct Well* well = malloc (sizeof (struct Well));
  well->endianness = 0; // 0 for neither, 1 for little, 2 for big.
  well->mutex = uthread_sem_create(1);
  well->impatient_big_size = 0;
  well->impatient_little_size = 0;
  well->bigWaiting = uthread_sem_create(0);
  well->littleWaiting = uthread_sem_create(0);
  well->occupancy = 0;
  well->impatient_big = uthread_sem_create(0);
  well->impatient_little = uthread_sem_create(0);
  well->max = uthread_sem_create(3);
  return well;
}

struct Person {
  enum Endianness endianness;
  int num_enters;
  int lineStart;                 // entry ticker once thread starts waiting to enter
  int waitTime;                  // entryTicker at time of entering well - lineStart
};

struct Person* createPerson(int random) {
  struct Person* p = malloc(sizeof(struct Person));
  p->endianness = oppositeEnd[random];
  p->num_enters = 0;
  p->lineStart = 0;
  return p;
}


#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t   waitingHistogramMutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];


int is_wait (int a, int b) {
  if (a == 0) {
    if (b == 2)
    return 1;
  } else {
    if (b == 1)
    return 1;
  }
  return 0;
}


void recordWaitingTime (int waitingTime) {
  uthread_sem_wait(waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal(waitingHistogramMutex);
}

// check occupancy
// check 
void enterwell (struct Person* p) {
  printf("entered well\n");
  uthread_sem_wait(well->max);
  uthread_sem_wait(well->mutex);
  p->lineStart = entryTicker;
  if (is_wait((int)p->endianness, well->endianness)) {
    printf("waiting\n");
    uthread_sem_signal(well->mutex);
    if (p->endianness == 1) {
      uthread_sem_wait(well->bigWaiting);
      printf("Exited big Q\n");
    }
    else {
      uthread_sem_wait(well->littleWaiting);
      printf("Exited little Q\n");
    }
    uthread_sem_wait(well->mutex);
  }
  if (well->occupancy == 0) {
    if (p->endianness == 1)
      well->endianness = 2;
    else
      well->endianness = 1;
  }
  printf("p endianness %d\n", p->endianness);
  p->waitTime = entryTicker - p->lineStart;
  recordWaitingTime(p->waitTime);
  well->occupancy++;
  printf("well endianness %d Well occupancy %d\n", well->endianness,well->occupancy);
  assert(well->occupancy <= 3);
  occupancyHistogram[well->endianness - 1][well->occupancy]++;
  entryTicker++;
  // have to add him to the well
  uthread_sem_signal(well->mutex);
  
}

void leavewell() {
  uthread_sem_wait(well->mutex);
  if (well->endianness == 2 && well->occupancy != 1) {
    // printf("Broadcasting to all BIGS\n");
      uthread_sem_signal(well->bigWaiting);
      printf("Signalled BIG Q\n");
    }
  else if (well->endianness == 1 && well->occupancy != 1) {
    // printf("Broadcasting to all SMALLS\n");
      uthread_sem_signal(well->littleWaiting);
      printf("Signalled LITTLE Q\n");
    }
  else {
      printf("ELSE CASE\n");
      //printf("Broadcasting to ALL\n");
      if (well->endianness == 1)
      uthread_sem_signal(well->bigWaiting);
      if (well->endianness == 2)
      uthread_sem_signal(well->littleWaiting);
  }
  printf("left\n");
  well->occupancy--;
  uthread_sem_signal(well->max);
  assert(well->occupancy >= 0);
  if (well->occupancy == 0)
    well->endianness = 0;
  uthread_sem_signal(well->mutex);
}

void* person(void* pv) {
  struct Person * p = pv;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    
    //printf("endianness %d iteration %d\n", p->endianness, i);
    enterwell(p);
    
    for (int j = 0; j < NUM_ITERATIONS; j++) 
      uthread_yield();
    
    printf("p endianness leaving %d\n",p->endianness);  
    leavewell();
    
    for (int k = 0; k < NUM_ITERATIONS; k++) 
      uthread_yield();
    
  }
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (4);
  well = createwell();
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
