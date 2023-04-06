%{
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "decl.h"

#define YYSTYPE char *
extern int yylex(void);
void yyerror(const char * message);
%}

%union
{
    int number;
    int op;
    char* str;
    char* node_name;
    char* attribute_name;
    bool boolean;
    double double_num;
    filters_list*  filters_l;
}

%token <double_num> DOUBLE_NUM
%token <number> NUMBER
%token <boolean> BOOL
%token <str> WORD

/* symbols */
%token SLASH START_FILTER END_FILTER OPEN_BRACKET CLOSE_BRACKET COMMA IS_ATTRIBUTE
/* filter tokens */
%token <op> EQUAL NOT_EQUAL LESS MORE
/* functions */
%token UPDATE REMOVE CREATE_EL CREATE_SCH

%type <str> attribute
%type <op> operator
%type <str> element
%type <str> node

%type <filters_l> filter_object
%type <filters_l> filter
%type <filters_l> filters
%%

query:
        |
        query element filters
         {
               print_filters_list($3);
         } |
        query element |
        query node |
        query separator attribute |
        func ;
func:
     query function OPEN_BRACKET query CLOSE_BRACKET
        {
                printf("\tfunction!\n");
        } ;

element:
    separator node
        {
                $$=$2;
                printf("node_name: %s", $$);
                print_newline();
        }
        ;
node:
        WORD ;
separator:
          SLASH ;
filters:
        filter | filters filter
            {
                    printf("\tfilter attribute node!\n");
                    $$ = $2;
                    $2->next = $1;
            } ;
filter:
            START_FILTER filter_object END_FILTER
            {
                    printf("\tfilter attribute node!\n");
                    $$ = $2;
            } ;

filter_object:
            WORD
            {
                $$ = add_filters_list(-1, add_int32_element($1), NULL);
            }
            |
            NUMBER
            {
                $$ = add_filters_list(-1, add_str_element($1), NULL);
            }
            |
            attribute operator WORD
            {
                $$ = add_filters_list(operator, add_str_element($3), attribute);
            }
            |
            attribute operator NUMBER
            {
                $$ = add_filters_list(operator, add_int32_element($3), attribute);
            }
            |
           attribute operator DOUBLE_NUM
            {
                $$ = add_filters_list(operator, add_double_element($3), attribute);
            }
            |
            attribute operator BOOL
            {
                $$ = add_filters_list(operator, add_bool_element($3), attribute);
            }
            ;
operator:
    EQUAL | NOT_EQUAL | LESS | MORE ;

function:
    UPDATE | REMOVE | CREATE_EL | CREATE_SCH;


attribute:
    IS_ATTRIBUTE WORD
    {
        printf("\tattribute!\n");
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
