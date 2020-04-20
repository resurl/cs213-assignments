//////
// This is part of the solution to UBC CPSC 213, Assignment 11.
// Do no share this code or any portion of it with anyone in any way.
// Do not remove this comment block.
//////

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

enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

struct Well {
  uthread_mutex_t     mutex;
  uthread_cond_t      canEnter [2];
  int                 occupantCount;
  enum Endianness occupantEndianness;
  int                 waitersCount [2];
  int                 outEndiannessWaitingCount;
};

struct Well* createWell() {
  struct Well* well = malloc (sizeof (struct Well));
  well->mutex                 = uthread_mutex_create();
  well->canEnter [LITTLE]       = uthread_cond_create (well->mutex);
  well->canEnter [BIG]     = uthread_cond_create (well->mutex);
  well->occupantCount         = 0;
  well->occupantEndianness        = 0;
  well->waitersCount [LITTLE]   = 0;
  well->waitersCount [BIG] = 0;
  well->outEndiannessWaitingCount = 0;
  return well;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogramMutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void enterWell (struct Well* well, enum Endianness Endianness) {
  uthread_mutex_lock (well->mutex);
    while (1) {
      int isEmpty            = well->occupantCount                           == 0;
      int hasRoom            = well->occupantCount                           <  MAX_OCCUPANCY;
      int sameEndianness     = well->occupantEndianness                      == Endianness;
      int oppositeEndWaiting = well->waitersCount [oppositeEnd [Endianness]] >  0;
      int waitingNotFair     = well->outEndiannessWaitingCount               >= FAIR_WAITING_COUNT;
      int oppositeEndsTurn   = oppositeEndWaiting && waitingNotFair;
      if (isEmpty || (hasRoom && sameEndianness && ! oppositeEndsTurn)) {
        if (sameEndianness)
          well->outEndiannessWaitingCount ++;
        else
          well->outEndiannessWaitingCount = 0;
        entryTicker ++;
        break;
      }
      VERBOSE_PRINT ("waiting to enter: %d %d %d %d %d %d\n", Endianness, isEmpty, hasRoom, sameEndianness, oppositeEndWaiting, waitingNotFair);
      if (! sameEndianness && well->waitersCount [Endianness] == 0)
        well->outEndiannessWaitingCount = 0;
      well->waitersCount [Endianness] ++;
      uthread_cond_wait (well->canEnter [Endianness]);
      well->waitersCount [Endianness] --;
    }
    VERBOSE_PRINT ("entering %d %d\n", Endianness, well->occupantCount+1);
    assert (well->occupantCount == 0 || well->occupantEndianness == Endianness);
    assert (well->occupantCount < MAX_OCCUPANCY);
    well->occupantEndianness = Endianness;
    well->occupantCount += 1;
    occupancyHistogram [well->occupantEndianness] [well->occupantCount] ++;
  uthread_mutex_unlock (well->mutex);
}

void leaveWell (struct Well* well) {
  uthread_mutex_lock (well->mutex);
    well->occupantCount -= 1;
    enum Endianness inEndianness  = well->occupantEndianness;
    enum Endianness outEndianness = oppositeEnd [inEndianness];
    int      outEndiannessWaiting     = well->waitersCount [outEndianness]  >  0;
    int      waitingNotFair       = well->outEndiannessWaitingCount     >= FAIR_WAITING_COUNT;
    int      inEndiannessWaiting      = well->waitersCount [inEndianness]   >  0;
    VERBOSE_PRINT ("leaving %d %d %d %d\n", inEndianness, outEndiannessWaiting, inEndiannessWaiting, waitingNotFair);
    if (outEndiannessWaiting && (waitingNotFair || ! inEndiannessWaiting)) {
      if (well->occupantCount == 0) {
        for (int i = 0; i < MAX_OCCUPANCY; i++) {
          VERBOSE_PRINT ("signalling out Endianness %d\n", outEndianness);
          uthread_cond_signal (well->canEnter [outEndianness]);
        }
      }
    } else if (inEndiannessWaiting) {
      VERBOSE_PRINT ("signalling in Endianness %d\n", inEndianness);
      uthread_cond_signal (well->canEnter [inEndianness]);
    }
  uthread_mutex_unlock (well->mutex);
}

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogramMutex);
}

void* person (void* wellv) {
  struct Well*    well = wellv;
  enum Endianness Endianness   = random() & 1;
  
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int startTime = entryTicker;
    enterWell (well, Endianness);
    recordWaitingTime (entryTicker - startTime - 1);
    for (int j = 0; j < NUM_PEOPLE; j++) uthread_yield();
    leaveWell (well);
    for (int j = 0; j < NUM_PEOPLE; j++) uthread_yield();
  }
  return NULL;
}

void mysrandomdev() {
  unsigned long seed;
  int f = open ("/dev/random", O_RDONLY);
  read    (f, &seed, sizeof (seed));
  close   (f);
  srandom (seed);
}

int main (int argc, char** argv) {
  uthread_init (1);
  mysrandomdev();
  struct Well* well = createWell();
  uthread_t        pt [NUM_PEOPLE];
  waitingHistogramMutex = uthread_mutex_create ();
  
  for (int i = 0; i < NUM_PEOPLE; i++)
    pt [i] = uthread_create (person, well);
  for (int i = 0; i < NUM_PEOPLE; i++)
    uthread_join (pt [i], 0);
    
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
