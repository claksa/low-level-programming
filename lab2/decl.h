#ifndef LAB2_DECL_H
#define LAB2_DECL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum QUERY {
    CREATE_SCHEME,
    CREATE_NODE,
    UPDATE,
    REMOVE,
    SELECT,
    SELECT_FILTER
} QUERY;

typedef enum {
    SELECTOR,
    OPERATION
} Path_type;

typedef enum FILTER {
    EQUAL = 0,
    NOT_EQUAL = 1,
    LESS = 2,
    MORE = 3
} FILTER;

typedef enum {
    INT32_TYPE,
    STRING_TYPE,
    DOUBLE_TYPE,
    BOOL_TYPE
} VALUE_TYPE;

typedef struct element {
    union {
        char* string_val;
        bool boolean;
        int32_t num;
        double double_num;
    };
    VALUE_TYPE element_type;
} element;

typedef struct {
    FILTER operator_val;
    element* val;
    char* attribute; /*optional*/
} Filter_obj;

typedef struct filters_list {
    Filter_obj* filt;
    filtes_list* next;
} filters_list;

typedef struct property {
    char* attr_name;
    element* el;
    property* next;
} property;

filters_list* add_filters_list(int op, element* val, char* attr);
void print_newline();
void print_filter_obj(Filter_obj* obj);
void print_filters_list(filters_list* list);
element* add_int32_element(int32_t val);
element* add_bool_element(bool val);
element* add_str_element(char* val);
element* add_double_element(double val);

#endif //LAB2_DECL_H
