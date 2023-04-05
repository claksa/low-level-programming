%{
#include <stdio.h>
#include <string.h>

extern int yylex(void);
void yyerror(const char * message);
%}

%token WORD NUMBER
/* symbols */
%token ASTERISK SLASH IS_ATTRIBUTE START_FILTER END_FILTER OPEN_BRACKET CLOSE_BRACKET
/* filter tokens */
%token EQUAL NOT_EQUAL LESS MORE
/* functions */
%token UPDATE REMOVE CREATE
%%

query:
        |
        query separator node filter |
        query separator node |
        query separator node filter filter |
        query node |
        query node filter |
        query node filter filter |
        query separator attribute |
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
filter:
            START_FILTER filter_object END_FILTER
            {
                    printf("\tfilter object attribute node!\n");
            }
             |
            START_FILTER NUMBER END_FILTER
            {
                    printf("\tfilter num attribute node!\n");
            }
            ;
filter_object:
            WORD |
            attribute operator WORD |
            attribute operator NUMBER ;

operator:
    EQUAL | NOT_EQUAL | LESS | MORE ;

function:
    UPDATE | REMOVE | CREATE ;

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

int main()
{
    yyparse();
    return 0;
}