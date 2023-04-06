#ifndef LAB2_DECL_H
#define LAB2_DECL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct element {
    union {
        char* string_val;
        bool boolean;
        int32_t num;
        double double_num;
    };
    int element_type;
};

struct Filter_obj {
    int operator_val;
    struct element* val;
    char* attribute; /*optional*/
    struct Filter_obj* next;
};


/*struct filters_list {
    struct Filter_obj* filt;
    struct filtes_list* next;
}*/

struct property {
    char* attr_name;
    struct element* el;
    struct property* next;
};

//struct filters_list * add_filters_list(struct Filter_obj* obj, struct filters_list* head);
struct Filter_obj* create_filter_obj(int op, char* attribute, struct element* el);

void print_newline();
void print_filter_obj(struct Filter_obj* obj);
//void print_filters_list(struct filters_list* list);

struct element* add_int32_element(int32_t val);
struct element* add_bool_element(int val);
struct element* add_str_element(char* val);
struct element* add_double_element(double val);

#endif //LAB2_DECL_H
