#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4
struct Well* well;

enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

struct Well {
  uthread_mutex_t mutex;
  int impatient_big_size;
  int impatient_little_size;
  int endianness;
  int occupancy;
  uthread_cond_t littleWaiting;
  uthread_cond_t bigWaiting;
  uthread_cond_t impatient_big;
  uthread_cond_t impatient_little;
};

struct Well* createwell() {
  struct Well* well = malloc (sizeof (struct Well));
  well->endianness = 0;                                       // 0 for neither, 1 for little, 2 for big.
  well->mutex = uthread_mutex_create();
  well->impatient_big_size = 0;                               // queue size for big endian priority queue
  well->impatient_little_size = 0;                            // queue size for little endian priority queue
  well->bigWaiting = uthread_cond_create(well->mutex);        // represents big endian people who are waiting to enter well
  well->littleWaiting = uthread_cond_create(well->mutex);     // big endian people who are waiting to enter well
  well->occupancy = 0;                                        // # of people in well (max. 3)
  well->impatient_big = uthread_cond_create(well->mutex);     // big endian people who have been waiting too long
  well->impatient_little = uthread_cond_create(well->mutex);  // little endian people who have been waiting too long
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
uthread_mutex_t waitingHistogrammutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogrammutex);
}

// determines if person endianness a matches well endianness b
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

// if well is full or if well endianness is different from person endianness,
// then wait depending on your endianness. else proceed
// updates state of program (well & person) accordingly
void enterWell (struct Person* p) {
  uthread_mutex_lock(well->mutex);
  p->lineStart = entryTicker;

  if (well->occupancy >= 3 || is_wait((int)p->endianness, well->endianness)) {
    if (p->endianness == BIG) {
        while (well->occupancy >= 3 || is_wait((int)p->endianness, well->endianness)) {
          if (entryTicker - p->lineStart >= FAIR_WAITING_COUNT) {
            well->impatient_big_size++;
            uthread_cond_wait(well->impatient_big);
            well->impatient_big_size--;
          }
          else
            uthread_cond_wait(well->bigWaiting);
        }
    } else {
        while (well->occupancy >= 3 || is_wait((int)p->endianness, well->endianness)) {
           if (entryTicker - p->lineStart >= FAIR_WAITING_COUNT) {
            well->impatient_little_size++;
            uthread_cond_wait(well->impatient_little);
            well->impatient_little_size--;
          }
          else
            uthread_cond_wait(well->littleWaiting);
        }
      }
  }

  // if well is empty then set its endianness accordingly
  if (well->occupancy == 0) {
    if (p->endianness == BIG)
      well->endianness = 2;
    else
      well->endianness = 1;
  }

  // update state of person & well
  p->waitTime = entryTicker - p->lineStart;
  recordWaitingTime(p->waitTime);
  well->occupancy++;
  assert(well->occupancy <= 3);
  occupancyHistogram[well->endianness - 1][well->occupancy]++;
  entryTicker++;
  uthread_mutex_unlock(well->mutex);
}

void leaveWell() {
  uthread_mutex_lock(well->mutex);
   if (well->impatient_little_size > 0 || well->impatient_big_size > 0) {
    if (well->impatient_big_size >= well->impatient_little_size)
    uthread_cond_signal(well->impatient_big);
    else
    uthread_cond_signal(well->impatient_little);
  } else {
    if (well->endianness == 2 && well->occupancy != 1) {
      uthread_cond_broadcast(well->bigWaiting);
    }
    else if (well->endianness == 1 && well->occupancy != 1){
      uthread_cond_broadcast(well->littleWaiting);
    }
    else {
      uthread_cond_broadcast(well->littleWaiting);
      uthread_cond_broadcast(well->bigWaiting);
      }
  }

  // update state of program
  well->occupancy--;
  assert(well->occupancy >= 0);
  if (well->occupancy == 0)
    well->endianness = 0;
  uthread_mutex_unlock(well->mutex);
}


void* person(void* pv) {
  struct Person* p = pv;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    enterWell(p);
    
    for (int j = 0; j < NUM_ITERATIONS; j++)
      uthread_yield();
      
    leaveWell();

    for (int k = 0; j < NUM_ITERATIONS; k++)
      uthread_yield();

    p->num_enters++;
  }
  return NULL;
}


int main (int argc, char** argv) {
  uthread_init (4);
  well = createwell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  for (int i = 0; i < NUM_PEOPLE; i++) {
    int n = random() % 2;
    struct Person* p = createPerson(n);
    pt[i] = uthread_create(person, p);
  }

  for (int i = 0; i < NUM_PEOPLE; i++) {
    uthread_join(pt[i],NULL);
  }
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
