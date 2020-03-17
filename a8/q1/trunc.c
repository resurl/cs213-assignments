#include "list.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

int int_parse (element_t q) {
  char** qe = (char**) q;
  char *endp;
  int value = strtol(*qe, &endp, 10);
  if (*qe[0] != '\0' && endp[0] == '\0') {
      return 1;
  }
}

int parse (element_t q) {
  char** qe = (char**) q;
  char *endp;
  int value = strtol(*qe, &endp, 10);
  return (!(*qe[0] != '\0' && endp[0] == '\0'));
}

void truncate(element_t* out_list, element_t len, element_t src) {
    //int** n = (int**) len; // in->data [i]
    int* n = (int*) len;
    char** in = (char**) src; // cha->data [i]
    char*** out = (char***) out_list; //
    if(*out == NULL)
    *out = malloc(sizeof(char**));
    if (**out == NULL)
    **out = malloc(sizeof(char*));
    //printf("%d", *n);
    //printf("%s", *in); 
    strncpy(**out,*in,*n);
}

void print(element_t a) {
    printf("%s ", *((char**) a));
}
void col_print(element_t a) {
    printf("%s \n", *((char**) a));
}

void int_print(element_t a) {
    printf("%d ", *((int*) a));
}

void convertToInt(element_t* out, element_t in) {
    char** input = (char**) in;
    char* endp;
    int value = strtol(*input,&endp,10);
    int** addr = (int**) out;
    *addr = malloc(sizeof(element_t));
    **addr = value;
}

int is_Zero(element_t e) {
    int* point = (int*) e;
    return (*point != 0);
}

void max_length(element_t* list, element_t out_element, element_t index){
//     int *a = av, *b = bv, **r = (int**) rv;
//   if (*r == NULL)
//     *r = malloc(sizeof(int));
//   **r = *a + *b;
    int** lelem =(int**) list;
    int* indx = (int*) index;
    int* out = (int*) out_element;
    
    if (out == NULL)
        out = malloc(sizeof(int));
     if(*indx > *out)
        *out = *indx; 
}


void destroyall_ints(element_t e) {
    free((int*)e);
}

void destroyall_strs(element_t e) {
    free(*(char**) e);
    free((char**)e);
}

int main(int argc, char** argv) {
    struct list* strings = list_create();
    struct list* strings2 = list_create();
    struct list* in = list_create();
    struct list* cha = list_create();

    for (int i = 1; i < argc; i++) {
        list_append(strings, &argv[i]);
    }

    //list_foreach(print,strings);

    list_filter(int_parse,in,strings);
    struct list* in2 = list_create();
    list_map1(convertToInt, in2, in);
    struct list* in3 = list_create();
    list_filter(is_Zero, in3, in2);
    list_filter(parse,cha,strings);
    /* list_foreach(print, cha);
    printf("strings\n");
    list_foreach(int_print,in3); // this prints put a bunch of int**/
    list_map2(truncate, strings2, in3,cha);

    list_foreach(col_print, strings2);
    list_foreach(print,strings2);
    int out = 0; 
    int* op = &out;
    list_foldl(max_length,(element_t*)&op,in3);
    printf("\n%d\n", out);

    list_destroy(strings);
    list_destroy(in);
    list_destroy(cha);
    list_destroy(in3);

    list_foreach(destroyall_ints,in2);
    list_foreach(destroyall_strs,strings2);

    list_destroy(in2);
    list_destroy(strings2);
} 



