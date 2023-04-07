#ifndef LAB2_MAIN_H
#define LAB2_MAIN_H

#include <stddef.h>
#include <stdbool.h>

#include "decl.h"

void print_el_for_single_filter(struct element* el) {
    printf("filter = select: ");
    print_newline();
    print_tab();
    if (el->element_type == 0) {
        printf(" --> element number: %d", el->num);
    }
    if (el->element_type == 2) {
        printf(" --> descendant schema name: %s", el->string_val);
    }
    print_newline();
}

void print_filters(struct Filter_obj* filter) {
        if (filter->filter->is_single_val) {
            print_el_for_single_filter(filter->filter->val);
        } else {
            print_filter_obj(filter);
        }
}

struct Filter_obj* create_single_filter_obj(struct element* el) {
    struct Filter_obj * obj = (struct Filter_obj*) malloc(sizeof(struct Filter_obj*));
    struct filter_scheme * f_sch = (struct filter_scheme*) malloc(sizeof(struct filter_scheme*));
    obj->filter = f_sch;
    if (obj == NULL) {
        fprintf(stderr, "error allocate filter_obj");
        print_newline();
        return NULL;
    }
    obj->filter->is_single_val = true;
    obj->filter->val = el;
    print_el_for_single_filter(obj->filter->val);
    return obj;
}

void print_property(struct  property_scheme* prop) {
    printf("property name: %s", prop->property_name);
    print_newline();
    printf("property type: ");
    print_newline();
    switch(prop->node_type) {
        case 0:
            printf("int32");
            break;
        case 1:
            printf("double");
            break;
        case 2:
            printf("string");
            break;
        case 3:
            printf("bool");
            break;
        default:
            printf("no such type");
            return;
    }
    print_newline();
}

struct Filter_obj* create_property_scheme_filter_obj(char* prop_name, int type) {
    struct Filter_obj * obj = (struct Filter_obj*) malloc(sizeof(struct Filter_obj*));
    struct property_scheme * prop = (struct property_scheme*) malloc(sizeof(struct property_scheme*));
    prop->node_type = type;
    strcpy(prop->property_name, prop_name);
    obj->prop = prop;
    if (obj == NULL) {
        fprintf(stderr, "error allocate filter_obj");
        print_newline();
        return NULL;
    }
    print_property(prop);
    return obj;
}

struct Filter_obj* create_filter_obj(int op, char* attribute, struct element* el) {
    struct Filter_obj * obj = (struct Filter_obj*) malloc(sizeof(struct Filter_obj*));
    struct filter_scheme * f_sch = (struct filter_scheme*) malloc(sizeof(struct filter_scheme*));
    obj->filter = f_sch;
    if (obj == NULL) {
        fprintf(stderr, "error allocate filter_obj");
        print_newline();
        return NULL;
    }
    obj->filter->is_single_val = false;
    obj->filter->val = el;
    obj->filter->operator_val = op;
    strcpy(obj->filter->attribute, attribute);
    print_filter_obj(obj);
    return obj;
}

void print_newline() {
    printf("\n");
}

void print_tab() {
    printf("\t");
}

void print_element(struct element* el) {
    print_tab();
    print_tab();
    switch (el->element_type) {
        case 0:
            printf(" value = %d" , el->num);
            break;
        case 1:
            printf(" value = %f" , el->double_num);
            break;
        case 2:
            printf(" value = %s", el->string_val);
            break;
        case 3:
            printf(" value = %s", el->boolean ? "true" : "false");
            break;
        default:
            printf("no type with this num = %d", el->element_type);
    }
    print_newline();
}

void print_operator(int operator_value) {
    printf("operator: ");
    print_newline();
    print_tab();
    switch(operator_value) {
        case 0:
            printf("equal to ");
            break;
        case 1:
            printf("not_equal to ");
            break;
        case 2:
            printf("more than");
            break;
        case 3:
            printf("less than");
            break;
        default:
            printf("not such operator :(");
    }
}

void print_filter_obj(struct Filter_obj* obj) {
    printf("filter = select property:");
    print_newline();
    print_tab();
    printf(" --> property name %s", obj->filter->attribute);
    print_newline();
    print_operator(obj->filter->operator_val);
    print_newline();
    print_element(obj->filter->val);
    print_newline();
}

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
