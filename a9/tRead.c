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
  unsigned int sum = 0;

  void interrupt_service_routine () {
    uthread_t thre;
    queue_dequeue(pending_read_queue, (void**) &thre, NULL, NULL);
    uthread_unblock(thre);
  }

  void *read_block (void *arg) {
    int count = 0;
    int result;
    queue_enqueue(pending_read_queue, uthread_self(), NULL, NULL);
    disk_schedule_read (&result, *(int*)arg);
    uthread_block();
    sum += result;
    return NULL;
  }

  int main (int argc, char** argv) {
    // Command Line Arguments
    static char* usage = "usage: tRead num_blocks";
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

    uthread_t* thread = malloc(sizeof(uthread_t) * num_blocks);
    int* arr = malloc(sizeof(int) * num_blocks);
    // Sum Blocks
    for (int blockno = 0; blockno < num_blocks; blockno++) {
      arr[blockno] = blockno;
      thread[blockno] = uthread_create(read_block, &arr[blockno]);
    }
    for (int i = 0; i < num_blocks; i++) {
      uthread_join(thread[i], NULL);
    }

    free(thread);
    free(arr);

    printf("%d\n", sum);
    return 0;
  }


