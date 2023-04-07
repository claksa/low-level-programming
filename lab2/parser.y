%{

#include "decl.h"

extern int yylex(void);
void yyerror(const char * message);
%}

%union
{
    int number;
    int bool_value;
    double double_num;

    char* str;

    struct Filter_obj* filt_obj;
    struct element* el;
}

%token <str> WORD
%token <number> NUMBER
%token <double_num> DOUBLE_NUM
%token <bool_value> BOOL

/* symbols */
%token SLASH START_FILTER END_FILTER OPEN_BRACKET CLOSE_BRACKET SEMICOLON IS_ATTRIBUTE

/* filter tokens */
%token <number> EQUAL NOT_EQUAL LESS MORE
%token <number> INT32_TYPE DOUBLE_TYPE STRING_TYPE BOOLEAN_TYPE

/* functions */
%token <str> UPDATE REMOVE_EL REMOVE_SCH CREATE_EL CREATE_SCH ASTERISK

%type <str> attribute
%type <str> node
%type <str> function

%type <number> operator
%type <number> val_type
%type <el> node_value

%type <filt_obj> filter_object
%type <filt_obj> filter
%type <filt_obj> filters
%%

query:
        |
        query node filters
        |
        query node
        |
        func ;
func:
     query function OPEN_BRACKET query CLOSE_BRACKET
        {
                printf("OPERATION: %s\n", $2);
        }
     |
     query ASTERISK
     {
            printf("%s", $2);
            print_newline();
     }
     ;

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
            |
            attribute SEMICOLON val_type
            {
                $$ = create_property_scheme_filter_obj($1, $3);
            }
            ;
node:
         separator WORD
         {
            printf("descendant: ");
            print_newline();
            print_tab();
            printf(" ---> shema name: %s", $2);
            print_newline();
         }
         |
         WORD
         {
            printf(" --> schema name: %s", $1);
            print_newline();
         }
         |
         separator IS_ATTRIBUTE WORD
         {
            print_newline();
            print_tab();
            printf("--> node property: %s", $3);
            print_newline();
         }
          ;
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
    UPDATE | REMOVE_EL | REMOVE_SCH | CREATE_EL | CREATE_SCH ;

operator:
    EQUAL | NOT_EQUAL | LESS | MORE ;

val_type:
    INT32_TYPE | DOUBLE_TYPE | STRING_TYPE | BOOLEAN_TYPE ;

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
