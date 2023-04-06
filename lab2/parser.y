%{

#include "decl.h"

extern int yylex(void);
void yyerror(const char * message);
%}

%union
{
    int number;
    int op;
    int type;
    int bool_value;
    double double_num;

    char* str;
    char* node_name;
    char* attribute_name;

    struct Filter_obj* filt_obj;
    struct element* el;
}

%token <str> WORD
%token <number> NUMBER
%token <double_num> DOUBLE_NUM
%token <bool_value> BOOL

/* symbols */
%token SLASH START_FILTER END_FILTER OPEN_BRACKET CLOSE_BRACKET COMMA IS_ATTRIBUTE ASTERISK

/* filter tokens */
%token <op> EQUAL NOT_EQUAL LESS MORE
%token <type> INT32_TYPE DOUBLE_TYPE STRING_TYPE BOOLEAN_TYPE

/* functions */
%token UPDATE REMOVE CREATE_EL CREATE_SCH

%type <str> attribute
%type <str> node

%type <op> operator
%type <type> val_type
%type <el> node_value

%type <filt_obj> filter_object
%type <filt_obj> filter
%type <filt_obj> filters
%%

query:
        |
        query separator node filters |
        query separator node |
        query node |
        query separator attribute |
        func ;
func:
     query function OPEN_BRACKET query CLOSE_BRACKET
        {
                printf("\tfunction!\n");
        } ;

filters:
        filter | filters filter
            {
                    $$ = $2;
                    $2->next = $1;
            } ;

filter:
            START_FILTER filter_object END_FILTER
            {
                    $$ = $2;
            } ;

filter_object:
            node_value
            {
                $$ = create_single_filter_obj($1);
            }
            |
            attribute operator node_value
            {
                $$ = create_filter_obj($2, $1, $3);
            }
            ;
node:
         WORD ;
separator:
          SLASH ;

node_value:
    NUMBER
    {
        $$ = add_int32_element($1);
    }
    |
     DOUBLE_NUM
     {
        $$ = add_double_element($1);
     }
     |
     WORD
     {
        $$ = add_str_element($1);
     }
     |
     BOOL
     {
        $$ = add_bool_element($1);
     }
     ;

function:
    UPDATE | REMOVE | CREATE_EL | CREATE_SCH ;

operator:
    EQUAL | NOT_EQUAL | LESS | MORE ;


attribute:
    IS_ATTRIBUTE WORD
    {
        $$ = $2;
    }
    ;

%%
void yyerror(const char *str)
{
        fprintf(stderr,"ошибка: %s\n",str);
        fprintf(stderr, "\n");
}

int yywrap()
{
        return 1;
}
