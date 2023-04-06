%{
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

extern int yylex(void);
void yyerror(const char * message);
%}

%union
{
    int number;
    char* string;
    _Bool boolean;
}

%token <number> NUMBER
%token <boolean> BOOL
%token <string> WORD
/* symbols */
%token ASTERISK SLASH IS_ATTRIBUTE START_FILTER END_FILTER OPEN_BRACKET CLOSE_BRACKET COMMA
/* filter tokens */
%token EQUAL NOT_EQUAL LESS MORE
/* functions */
%token UPDATE REMOVE CREATE_EL CREATE_SCH
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
node:
        WORD
        {
                printf("\tword!\n");
        }
        |
        ASTERISK
        {
                printf("\tall!\n");
        }
        ;
separator:
          SLASH
          {
                printf("\tslashh\n");
          }
          ;
filters:
        filter | filters filter;
filter:
            START_FILTER filter_object END_FILTER
            {
                    printf("\tfilter attribute node!\n");
            } ;

filter_object:
            WORD |
            NUMBER |
            attribute operator WORD |
            attribute operator NUMBER |
            attribute operator bool_val;

operator:
    EQUAL | NOT_EQUAL | LESS | MORE ;

function:
    UPDATE | REMOVE | CREATE_EL | CREATE_SCH;

bool_val:
        BOOL
        {
              printf("\tboolean!\n");
        }
        ;

attribute:
    IS_ATTRIBUTE WORD
    {
        printf("\tattribute!\n");
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
