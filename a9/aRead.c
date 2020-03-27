#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t      pending_read_queue;
volatile unsigned int sum = 0;
int counter = 0;
// You may add your own variables here

void interrupt_service_routine () {
  void* val;
  void (*callback)(void*,void*);
  queue_dequeue (pending_read_queue, &val, NULL, &callback);
  callback (val, NULL);
}

void handle_read (void* resultv, void* not_used) {
  sum += *(int*)resultv;
  counter++;
}

int main (int argc, char** argv) {
  // Command Line Arguments
  static char* usage = "usage: aRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  // Sum Blocks
  // add element to list
   for (int blockno = 0; blockno < num_blocks; blockno++) {
     int result;
     queue_enqueue(pending_read_queue, &result, NULL, handle_read);
     disk_schedule_read (&result, blockno);
   }
   while (counter != num_blocks);
  printf("%d\n", sum);
  return 0;
}
