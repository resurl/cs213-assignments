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

void truncate(element_t* e_addr, element_t len, element_t src) {
    int* n = (int*) len; // fil_in [i] this is fine 
    char** fil_in = (char**) src; // cha [i] fine
    char*** out = (char***) e_addr;
    if (*out == NULL)
        *out = malloc(sizeof(char*));
    if (**out == NULL)
        **out = malloc(sizeof(char*) * strlen(*fil_in));
    strncpy(**out, *fil_in, *n);
}

void destroyElem(element_t e) {
    free(*((char**)e));
    free(e);
}

void destroy_strs(struct list* list) {
    list_foreach(destroyElem, list);
    list_destroy(list);
}

void destroyint(element_t e) {
    free((int*)e);
}

void destroy_ints(struct list* list) {
    list_foreach(destroyint, list);
    list_destroy(list);
}

void print(element_t a) {
    printf("%s ", *((char**) a));
}

void int_print(element_t a) {
    printf("%d ", *((int*) a));
}

void convertToInt(element_t* out, element_t in) {
    char** elm = (char**) in;
    int** conv = (int**) out;
    char* endp;
    if (*conv == NULL)
        *conv = malloc(sizeof(int));
    int value = strtol(*elm ,&endp, 10); 
    **conv = value;
}


int main(int argc, char** argv) {

    struct list* strings = list_create();
    struct list* truncd = list_create();
    struct list* fil_in = list_create();
    struct list* ins = list_create();
    struct list* cha = list_create();

    for (int i = 1; i < argc; i++) {
        list_append(strings, &argv[i]); //strings has list (data)of char ** 
    }

    list_foreach(print,strings);
    list_filter(int_parse,fil_in,strings); // fil_in contains all the "ints" fil_in strings as char* (confirmed)
    list_filter(parse,cha,strings); // cha contains all the char* fil_in strings (confirmed)

    list_map1(convertToInt, ins, fil_in); // contains all ints as ints
    //list_foreach(int_print,ins);

    list_map2(truncate, truncd, ins,cha);
    list_foreach(print, truncd);


    list_destroy(strings);
    list_destroy(cha);
    list_destroy(fil_in);
    
    destroy_ints(ins);
    destroy_strs(truncd);
} 



