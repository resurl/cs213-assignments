CFLAGS += -std=gnu11 -g
EXES    = greet BinaryTree

all:  $(EXES)
clean:
	rm -f $(EXES)

greet: greet.c
BinaryTree: BinaryTree.c
# don't treat all and clean as file targets
.PHONY: all clean
