%{
#include <stdio.h>
#include "decl.h"
#include "y.tab.h"

%}
%%
"update"               {return UPDATE;}
"remove"               {return REMOVE;}
"create_element"        {return CREATE_EL;}
"create_schema"         {return CREATE_SCH;}


([+-]?[0-9])+                    {yylval.number = atoi(yytext); return NUMBER;}
[+-]?([0-9]*[.])?[0-9]+          {yylval.number = atof(yytext); return DOUBLE_NUM;}
"true"                           {yylval.boolean = true; return BOOL;}
"false"                          {yylval.boolean = false; return BOOL;}
[a-zA-Z][a-zA-Z0-9]*             {yylval.str = strdup(yytext); return WORD;}

"="                      {yylval.op = 0; return EQUAL;}
"!="                     {yylval.op = 1; return NOT_EQUAL;}
">"                      {yylval.op = 2; return MORE;}
"<"                      {yylval.op = 3; return LESS;}
"/"                      {return SLASH;}
@                        {return IS_ATTRIBUTE;}
"*"                      {return ASTERISK;}
"["                      {return START_FILTER;}
"]"                      {return END_FILTER;}
"("                      {return OPEN_BRACKET;}
")"                      {return CLOSE_BRACKET;}
","                      {return COMMA;}
\n                      /* skip */;
[ \t]+                  /* skip */;
%%