UTHREAD = ./uthreads
TARGETS = q1 q2 q3 q4 pc

OBJS = $(UTHREAD)/uthread.o $(UTHREAD)/uthread_mutex_cond.o $(UTHREAD)/uthread_sem.o
JUNKF = $(OBJS) *~
JUNKD = *.dSYM
CFLAGS  += -g -std=gnu11 -I$(UTHREAD)
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -pthread
endif
all: $(TARGETS)
$(TARGETS): $(OBJS)
tidy:
	rm -f $(JUNKF); rm -rf $(JUNKD)
clean:
	rm -f $(JUNKF) $(TARGETS); rm -rf $(JUNKD)


