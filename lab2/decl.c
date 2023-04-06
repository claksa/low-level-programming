#ifndef LAB2_MAIN_H
#define LAB2_MAIN_H

#include <stddef.h>
#include <stdbool.h>

#include "decl.h"


//struct filters_list * add_filters_list(struct Filter_obj* obj, struct filters_list* head) {
//    struct filters_list * list = (struct filters_list*) malloc(sizeof(struct filters_list*));
//    if (list == NULL) return NULL;
//    list->filt = obj;
//    list->next = head;
//    head = list;
//    return head;
//}

struct Filter_obj* create_filter_obj(int op, char* attribute, struct element* el) {
    struct Filter_obj * obj = (struct Filter_obj*) malloc(sizeof(struct Filter_obj*));
    if (obj == NULL) return NULL;
    strcpy(obj->attribute, attribute);
    obj->val = el;
    obj->operator_val = op;
    return obj;
}

void print_newline() {
    printf("\n");
}

void print_filter_obj(struct Filter_obj* obj) {
    if (obj->val != NULL) {
        printf("element value exist!");
        print_newline();
    }
    if (obj->attribute != NULL) {
        printf("attribute name exist: ");
        print_newline();
        printf("attribute: %s", obj->attribute);
        print_newline();
    }
}

//void print_filters_list(struct filters_list* list) {
//    struct filters_list* current_filter = list;
//    while(current_filter != NULL) {
//        print_filter_obj(current->filt);
//        current_filter = current_filter->next;
//    }
//}


struct element* add_int32_element(int32_t val) {
    struct element* el = malloc(sizeof(struct element));
    if (el==NULL) return NULL;
    el->num = val;
    el->element_type = 0;
    return el;
}

struct element* add_double_element(double val) {
    struct element* el = malloc(sizeof(struct element));
    if (el==NULL) return NULL;
    el->double_num = val;
    el->element_type = 1;
    return el;
}

struct element* add_str_element(char* val) {
    struct element* el = malloc(sizeof(struct element));
    if (el==NULL) return NULL;
    strcpy(el->string_val, val);
    el->element_type = 2;
    return el;
}

struct element* add_bool_element(int val) {
    struct element* el = malloc(sizeof(struct element));
    if (el==NULL) return NULL;
    if (val==1) el->boolean = true;
    else el->boolean = false;
    el->element_type = 3;
    return el;
}

#endif //LAB2_MAIN_H
