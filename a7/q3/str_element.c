#include "str_element.h"
#include "refcount.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Implement all public int_element functions, including element interface functions.

You may add your own private functions here too. */
struct str_element_class {
    void (*print) (struct element *);
    int (*compare) (struct element*, struct element*);
    int (*is_str_element) (struct element *);
    char* (*str_element_get_value) (struct str_element *);
    struct str_element* (*str_element_new) (char*);
};

struct str_element {
    struct str_element_class* class;
    char* string;
};

void str_print(struct element *e);
int str_compare(struct element *e, struct element *e2);
int is_str_element(struct element *elm);
char* str_element_get_value(struct str_element *elm);
struct str_element *str_element_new(char* str);

struct str_element_class str_element_class = {str_print,str_compare,is_str_element,str_element_get_value,str_element_new};

void str_print(struct element *e) {
    struct str_element* strElem = (struct str_element *) e;
    printf("%s", strElem->string);
}

int str_compare(struct element *e, struct element *e2) {
    struct str_element* e_cast = (struct str_element* ) e;
    if (is_str_element(e2)) {
        struct str_element* e2_s = (struct str_element*) e2;
        return strcmp(e_cast->string,e2_s->string);
    } else { 
        return 1;
    }
}

int is_str_element(struct element *elm) {
    return (((struct str_element*)elm)->class == &str_element_class);   
}

char* str_element_get_value(struct str_element *elm) {
    return elm->string;
}

void str_finalizer(void* e) {
    struct str_element* elm = (struct str_element*) e;
    free(elm->string);
}

struct str_element *str_element_new(char* str) {
    struct str_element *elmt = rc_malloc(sizeof(struct str_element), str_finalizer);
    elmt->class = &str_element_class;
    elmt->string = strdup(str);
    return elmt;
}