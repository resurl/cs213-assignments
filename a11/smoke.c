#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 10

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

int tobSig = 0;
int papSig = 0;
int matSig = 0;

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
  /* uthread_cond_t  signal_go;
  uthread_cond_t  smoker_go; */
  uthread_cond_t  matSmoke;
  uthread_cond_t  papSmoke;
  uthread_cond_t  tobSmoke;
};

struct Manager {
    struct Agent* agent;
    int combi;
};

struct Smoker {
  struct Manager* manager;
};

struct Smoker* createSmoker(struct Manager* manager) {
  struct Smoker* smoker = malloc(sizeof(struct Smoker));
  smoker->manager = manager;
}

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  /* agent->signal_go = uthread_cond_create(agent->mutex);
  agent->smoker_go = uthread_cond_create(agent->mutex); */
  agent->matSmoke   = uthread_cond_create (agent->matSmoke);
  agent->tobSmoke   = uthread_cond_create (agent->tobSmoke);
  agent->papSmoke   = uthread_cond_create (agent->papSmoke);
  return agent;
}
//
// TODO
// You will probably need to add some procedures and struct etc.
//

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
struct Manager* createManager(struct Agent* agent, int i) {
    struct Manager* manager = malloc(sizeof (struct Manager));
    manager->agent = agent;
    manager->combi = i;
    return manager;
}

void* manager (void* mv) {
    printf("manager start\n");
    struct Manager* m = (struct Manager*) mv;
    uthread_mutex_lock(m->agent->mutex);
    printf("hello....");
    //for (int i = 0; i < NUM_ITERATIONS; i++) {
    printf("manager %d iteration %d", m->combi, i);
    switch(m->combi) {
        case TOBACCO:
        while (tobSig == 0)
          uthread_cond_wait(m->agent->tobacco);
        tobSig++;
        break;
        case MATCH:
        while (matSig == 0)
        uthread_cond_wait(m->agent->match);
        matSig++;
        break;
        case PAPER:
        while (papSig == 0)
        uthread_cond_wait(m->agent->paper);
        papSig++;
        break;
        default:
        break;
    }
    uthread_cond_signal(m->agent->smoker_go);
    //}
    uthread_mutex_unlock(m->agent->mutex);
}

void* smoker(void * av) {
  printf("smoker start\n");
  struct Smoker* smoke = av;
  uthread_mutex_lock(smoke->manager->agent->mutex);
  printf("smoke lock");
  for (int i = 0; i < NUM_ITERATIONS; i++) {
  switch (smoke->manager->combi) {
    case PAPER:
    while (!matSig && !tobSig)
      uthread_cond_wait()
    if (matSig && tobSig) {
      matSig--;
      tobSig--;
      uthread_cond_signal(smoke->manager->agent->smoke);
    }
    break;
    case MATCH:
    if (papSig && tobSig) {
      papSig--;
      tobSig--;
      uthread_cond_signal(smoke->manager->agent->smoke);
    }
    break;
    case TOBACCO:
    if (matSig && papSig--) {
      matSig--;
      papSig--;
      uthread_cond_signal(smoke->manager->agent->smoke);
    }
    break;
    default:
    break;
  }
  }
  uthread_mutex_unlock(smoke->manager->agent->mutex);
}

void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
   printf("agent start");
  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      
      printf("iteration %d\n", i);
      int r = random() % 3;
      signal_count [matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        printf("match\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        printf("paper\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        printf("tobacco\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (7);
  struct Agent*  a = createAgent();
  struct Manager* tobMan = createManager(a,TOBACCO);
  struct Manager* matMan = createManager(a,MATCH);
  struct Manager* papMan = createManager(a,PAPER);
  struct Smoker* tobSmok = createSmoker(tobMan);
  struct Smoker* matSmok = createSmoker(matMan);
  struct Smoker* papSmok = createSmoker(papMan);
  uthread_t one = uthread_create(manager(tobMan), 0);
  uthread_t two = uthread_create(manager(matMan), 0);
  uthread_t three = uthread_create(manager(matMan), 0);
  uthread_t four = uthread_create(smoker(tobSmok), 0);
  uthread_t five = uthread_create(smoker(matSmok), 0);
  uthread_t six = uthread_create(smoker(papSmok), 0);
  uthread_join (uthread_create (agent, a), 0);
  /* uthread_join(one, 0);
  uthread_join(two, 0);
  uthread_join(three, 0);
  uthread_join(four, 0);
  uthread_join(five, 0);
  uthread_join(six, 0); */
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}