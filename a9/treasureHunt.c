#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

queue_t pending_read_queue;
int counter = 0;

void interrupt_service_routine() {
  void* val;
  void (*callback)(void*,void*);
  void* arg;
  queue_dequeue (pending_read_queue, &val, &arg, &callback);
  callback (val, arg);
}

void handleOtherReads(void *resultv, void *countv) {
  *(int*) countv += 1;
}

void handleFirstRead(void *resultv, void *countv) {
  *(int*) countv = *(int *)resultv;
}

int main(int argc, char **argv) {
  // Command Line Arguments
  static char* usage = "usage: treasureHunt starting_block_number";
  int starting_block_number;
  char *endptr;
  if (argc == 2)
    starting_block_number = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  int num_reads = 0;
  int result = 0;
  queue_enqueue(pending_read_queue, &result, &num_reads, handleFirstRead);
  disk_schedule_read(&result, starting_block_number);
  while(num_reads == 0);

  for(int i = 0; i < num_reads; i++) {
    queue_enqueue(pending_read_queue, &result, &counter, handleOtherReads);
    disk_schedule_read(&result, result);
    while (counter == i);
    if (counter == num_reads) 
      printf("%d", result);
  }
}
