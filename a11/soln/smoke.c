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

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}

struct Pusher {
  uthread_cond_t    matchPaper;
  uthread_cond_t    matchTobacco;
  uthread_cond_t    paperTobacco;
  uthread_cond_t    smoke;
  struct Agent*     agent;
  int               paper, match, tobacco;
};

struct Pusher* createPusher (struct Agent* agent) {
  struct Pusher* pusher = malloc (sizeof (struct Pusher));
  pusher->matchPaper   = uthread_cond_create (agent->mutex);
  pusher->matchTobacco = uthread_cond_create (agent->mutex);
  pusher->paperTobacco = uthread_cond_create (agent->mutex);
  pusher->smoke        = uthread_cond_create (agent->mutex);
  pusher->agent        = agent;
  pusher->paper        = 0;
  pusher->match        = 0;
  pusher->tobacco      = 0;
  return pusher;
}

enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];
int smoke_count  [5];

void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
  
  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      int r = random() % 3;
      signal_count [matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

void pusher (struct Pusher* p, enum Resource r, uthread_cond_t waitCond, int* isAvailable) {
  uthread_mutex_lock (p->agent->mutex);
    while (1) {
      VERBOSE_PRINT ("waiting for %s\n", resource_name [r]);
      uthread_cond_wait (waitCond);
      VERBOSE_PRINT ("have %s\n", resource_name [r]);
      *isAvailable = 1;
      if (p->match && p->paper) {
        p->match = p->paper = 0;
        VERBOSE_PRINT ("signalling smoker with match and paper\n");
        uthread_cond_signal (p->matchPaper);
        VERBOSE_PRINT ("pusher waiting for smoker\n");
        uthread_cond_wait   (p->smoke);
        VERBOSE_PRINT ("pusher signalling agent\n");
        uthread_cond_signal (p->agent->smoke);
      } else if (p->match && p->tobacco) {
        p->match = p->tobacco = 0;
        VERBOSE_PRINT ("signalling smoker with match and tobacco\n");
        uthread_cond_signal (p->matchTobacco);
        VERBOSE_PRINT ("pusher waiting for smoker\n");
        uthread_cond_wait   (p->smoke);
        VERBOSE_PRINT ("pusher signalling agent\n");
        uthread_cond_signal (p->agent->smoke);
      } else if (p->paper && p->tobacco) {
        p->paper = p->tobacco = 0;
        VERBOSE_PRINT ("signalling smoker with paper and tobacco\n");
        uthread_cond_signal (p->paperTobacco);
        VERBOSE_PRINT ("pusher waiting for smoker\n");
        uthread_cond_wait   (p->smoke);
        VERBOSE_PRINT ("pusher signalling agent\n");
        uthread_cond_signal (p->agent->smoke);
      }
    }
  uthread_mutex_unlock (p->agent->mutex);
}

void* matchPusher (void* pv) {
  struct Pusher* p = pv;
  pusher (p, MATCH, p->agent->match, &p->match);
  return NULL;
}

void* paperPusher (void* pv) {
  struct Pusher* p = pv;
  pusher (p, PAPER, p->agent->paper, &p->paper);
  return NULL;
}

void* tobaccoPusher (void* pv) {
  struct Pusher* p = pv;
  pusher (p, TOBACCO, p->agent->tobacco, &p->tobacco);
  return NULL;
}

void smoke (struct Pusher* p, enum Resource r, uthread_cond_t waitCond) {
  uthread_mutex_lock (p->agent->mutex);
    while (1) {
      VERBOSE_PRINT ("smoker with %s is waiting\n", resource_name [r]);
      uthread_cond_wait (waitCond);
      VERBOSE_PRINT ("smoker with %s is smoking\n", resource_name [r]);
      smoke_count [r] ++;
      uthread_cond_signal (p->smoke);
    }
  uthread_mutex_unlock (p->agent->mutex);
}

void* smokeWithMatch (void* pv) {
  struct Pusher* p = pv;
  smoke (p, MATCH, p->paperTobacco);
  return NULL;
}

void* smokeWithPaper (void* pv) {
  struct Pusher* p = pv;
  smoke (p, PAPER, p->matchTobacco);
  return NULL;
}

void* smokeWithTobacco (void* pv) {
  struct Pusher* p = pv;
  smoke (p, TOBACCO, p->matchPaper);
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
  uthread_init (7);
  struct Agent*  a = createAgent();
  struct Pusher* p = createPusher (a);
  mysrandomdev();
  uthread_create (matchPusher,      p);
  uthread_create (paperPusher,      p);
  uthread_create (tobaccoPusher,    p);
  uthread_create (smokeWithMatch,   p);
  uthread_create (smokeWithPaper,   p);
  uthread_create (smokeWithTobacco, p);
  uthread_join   (uthread_create (agent, a), 0);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}
