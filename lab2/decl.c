#ifndef LAB2_MAIN_H
#define LAB2_MAIN_H

#include "decl.h"


filters_list* add_filters_list(int op, element* val, char* attr) {
    filters_list* filter = malloc(sizeof(filters_list));
    if (filter == NULL) return NULL;
    Filter_obj* filt = malloc(sizeof(Filter_obj));
    if (filt == NULL) return NULL;
    filt->val = val;
    if (attr!= NULL) strcpy(filt->attribute, attr);
    if (op!=-1) filt->operator_val = op;
    return filter;
}

void print_newline() {
    printf("\n");
}

void print_filter_obj(Filter_obj* obj) {
    if (obj->val != NULL) {
        printf("element value exist!");
        print_newline();
    }
    if (obj->attribute != NULL) {
        printf("attribute name exist: ");
        print_newline();
        print("attribute: %s", obj->attribute);
        print_newline();
    }
}

void print_filters_list(filters_list* list) {
    while(list->next != NULL) {
        print_filter_obj(list->filt);
    }
}


element* add_int32_element(int32_t val) {
    element* el = malloc(sizeof(element));
    if (el==NULL) return NULL;
    el->element_type = INT32_TYPE;
    el->num = val;
    return el;
}

element* add_bool_element(bool val) {
    element* el = malloc(sizeof(element));
    if (el==NULL) return NULL;
    el->element_type = BOOL_TYPE;
    el->boolean = val;
    return el;
}

element* add_str_element(char* val) {
    element* el = malloc(sizeof(element));
    if (el==NULL) return NULL;
    el->element_type = STRING_TYPE;
    strcpy(el->string_val, val);
    return el;
}

element* add_double_element(double val) {
    element* el = malloc(sizeof(element));
    if (el==NULL) return NULL;
    el->element_type = DOUBLE_TYPE;
    el->double_num = val;
    return el;
}

#endif //LAB2_MAIN_H
