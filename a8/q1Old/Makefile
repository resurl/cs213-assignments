CFLAGS += -std=gnu11 -g
EXES = test trunc
OBJS = list.o test.o 

all: $(EXES)

test:   test.o list.o
trunc:  trunc.o list.o

clean:
	rm -f $(EXES) $(OBJS)
	rm -rf $(LIBS)
tidy:
	rm -f $(OBJS)
