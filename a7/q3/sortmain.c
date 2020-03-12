#include <stdio.h>
#include <stdlib.h>

#include "int_element.h"
#include "str_element.h"
#include "element.h"

#include "refcount.h"

/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}

int comparator(const void* p, const void* q) {
  struct element** pe = (struct element**) p;
  struct element** qe = (struct element**) q;
  return ((*pe)->class->compare(*pe,*qe));
}

int main(int argc, char **argv) {
  /* TODO: Read elements into a new array using parse_string */
  struct element** elem_list = malloc(sizeof(*elem_list)*(argc-1));
  for (int i=0; i<argc-1; i++) {
    struct element *e = parse_string(argv[i+1]);  
    elem_list[i] = e;
  }
// Size of list = argc - 1;

int lsize = argc - 1;
qsort(elem_list,lsize,sizeof(struct element*), comparator);

  /* TODO: Sort elements with qsort */
  printf("Sorted: ");
  /* TODO: Print elements, separated by a space */
  for (int i = 0; i < lsize; i++) {
    elem_list[i]->class->print(elem_list[i]);
    printf(" ");
    rc_free_ref(elem_list[i]);
  }

  free(elem_list);
  return 0;
}


