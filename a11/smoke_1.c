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
  printf("created agaent");
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}

struct Smoker{
  uthread_mutex_t t;
  uthread_cond_t cond1;
  uthread_cond_t cond2;
  uthread_cond_t smoke;
};

int switcher = 1;
int is_smoke = 1;

struct Smoker* createSmoker(int i, struct Agent* a) {
  printf("created smoker");
  struct Smoker* smoker = malloc(sizeof(struct Smoker));
  smoker->t = a->mutex;
  smoker->smoke = a->smoke;
  switch (i)
  {
  case 1:
    smoker->cond1 = a->paper;
    smoker->cond2 = a->tobacco;
    break;
  case 2:
    smoker->cond1 = a->match;
    smoker->cond2 = a->tobacco;
    break;
  case 4:
    smoker->cond1 = a->match;
    smoker->cond2 = a->paper;
    break;
  default:
    break;
  }
  return smoker;
}


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
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
  
  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      is_smoke = 1;
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
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
      is_smoke = 0;
      uthread_cond_broadcast(a->match);
      uthread_cond_broadcast(a->paper);
      uthread_cond_broadcast(a->tobacco);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

void* smoker(void* av) {
  printf("Entered smoker");
  struct Smoker* s = av;
  uthread_cond_t con1 = s->cond1;
  uthread_cond_t con2 = s->cond2;
  uthread_mutex_lock(s->t);
  //while(1){
  uthread_cond_wait(con1);
  uthread_cond_wait(con2);
  switcher = 0;
  if (is_smoke)
  uthread_cond_signal(s->smoke);
 // }
  uthread_mutex_unlock(s->t);
}

void* resource_manager(int i, void* av) {
  printf("%d", i);
  struct Agent* a = av;
  while (1) {
  switch (i)
  {
  case 1:
    uthread_cond_wait(a->paper);
    while(switcher) {
      uthread_cond_signal(a->paper);
    }
    break;
  case 2:
    uthread_cond_wait(a->tobacco);
    while(switcher) {
      uthread_cond_signal(a->tobacco);
    }
    break;
  case 4:
    uthread_cond_wait(a->tobacco);
    while(switcher) {
      uthread_cond_signal(a->tobacco);
    }
    break;
  default:
    break;
  }
  }
  return NULL;  
}



int main (int argc, char** argv) {
  uthread_init (7);
  
  struct Agent*  a = createAgent();
  struct Smoker* match = createSmoker(1,a);
  struct Smoker* paper = createSmoker(2,a);
  struct Smoker* tobacco = createSmoker(4,a);
  printf("lol\n");
  uthread_t match1 = uthread_create(smoker(match), 0);
  printf("lol\n");
  uthread_t paper1 = uthread_create(smoker(paper), 0);
  printf("lol\n");
  uthread_t tobacco1 = uthread_create(smoker(tobacco), 0);
  printf("lol\n");
  uthread_t mp = uthread_create(resource_manager(1, a),0); // todo
  printf("lol\n");
  uthread_t mt = uthread_create(resource_manager(2, a),0);
  printf("lol\n");
  uthread_t pt = uthread_create(resource_manager(3, a),0);
  printf("lol\n");
  uthread_join (uthread_create (agent, a), 0);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}