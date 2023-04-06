#ifndef LAB2_MAIN_H
#define LAB2_MAIN_H

typedef enum QUERY {
    CREATE_SCHEME,
    CREATE_NODE,
    UPDATE,
    REMOVE,
    SELECT,
    SELECT_FILTER
} QUERY;

typedef enum FILTER {
    EQUAL,
    NOT_EQUAL,
    LESS,
    MORE,
    LESS_OR_EQUAL,
    MORE_OR_EQUAL
} FILTER;



#endif //LAB2_MAIN_H
