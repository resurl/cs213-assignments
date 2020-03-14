#include "list.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

int int_parse (element_t q) {
  char** qe = (char**) q;
  char *endp;
  int value = strtol(*qe, &endp, 10);
  return (*qe[0] != '\0' && endp[0] == '\0');
}

int parse (element_t q) {
  char** qe = (char**) q;
  char *endp;
  int value = strtol(*qe, &endp, 10);
  return (!(*qe[0] != '\0' && endp[0] == '\0'));
}

void truncate(element_t* out_list, element_t len, element_t src) {
    int* n = (int*) len; // in [i]
    char** in = (char**) src; // cha [i]
    char*** out = (char***) out_list;
    strncpy(**out,*in,*n);
}

void print(element_t a) {
    printf("%s", *((char**) a));
}

void int_print(element_t a) {
    printf("%d", *((int*) a));
}


int main(int argc, char** argv) {

    struct list* strings = list_create();
    struct list* strings2 = list_create();
    struct list* in = list_create();
    struct list* cha = list_create();

    for (int i = 1; i < argc; i++) {
        list_append(strings, &argv[i]);
    }

    list_foreach(print,strings);

    list_filter(int_parse,in,strings);
    list_filter(parse,cha,strings);
    list_foreach(int_print,in);

    list_map2(truncate, strings2, in,cha);

    list_foreach(print,strings2);
} 



