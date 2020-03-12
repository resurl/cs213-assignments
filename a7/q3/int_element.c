#include "int_element.h"
#include "refcount.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Implement all public int_element functions, including element interface functions.

You may add your own private functions here too. */
struct int_element_class {
    void (*print) (struct element *);
    int (*compare) (struct element*, struct element*);
    int (*is_int_element) (struct element *);
    int (*int_element_get_value) (struct int_element *);
    struct int_element* (*int_element_new) (int);
};

struct int_element {
    struct int_element_class *class;
    int i;
};

struct int_element *int_element_new(int value);
void int_print(struct element *e);
int int_compare(struct element *e, struct element *e2);
int is_int_element(struct element *elm);
int int_element_get_value(struct int_element *elm);
void int_finalizer(void* e);

struct int_element_class int_element_class = {int_print,int_compare,is_int_element,int_element_get_value,int_element_new};

int intcmp(int a, int b) {
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else 
        return 0;
}

void int_print(struct element *e) {
    struct int_element* intElem = (struct int_element *) e;
    printf("%d", intElem->i);
}

int int_compare(struct element *e, struct element *e2) {
    if (is_int_element(e2)) {
        struct int_element* int_elem1 = (struct int_element*) e;
        struct int_element* int_elem2 = (struct int_element*) e2;
        return (intcmp(int_elem1->i, int_elem2->i));
    } else {
        return -1;
    }
}

int is_int_element(struct element *elm) {
    return (((struct int_element*) elm)->class == &int_element_class);
}

int int_element_get_value(struct int_element *elm) {
    return elm->i;
}


void int_finalizer(void* e) {
    
}

struct int_element *int_element_new(int value) {
    struct int_element *elmt = rc_malloc(sizeof(struct int_element), int_finalizer);
    elmt->class = &int_element_class;
    elmt->i = value;
    return elmt;
}

