%{

#include "decl.h"
#include "y.tab.h"

%}
%%
"update"               {return UPDATE;}
"remove"               {return REMOVE;}
"create_element"        {return CREATE_EL;}
"create_schema"         {return CREATE_SCH;}

"type=int32"     {yylval.type = 0; return INT32_TYPE;}
"type=double"    {yylval.type = 1; return DOUBLE_TYPE;}
"type=string"    {yylval.type = 2; return STRING_TYPE;}
"type=bool"      {yylval.type = 3; return BOOLEAN_TYPE;}

"true"                           {yylval.bool_value = 1; return BOOL;}
"false"                          {yylval.bool_value = 0; return BOOL;}
([+-]?[0-9])+                    {yylval.number = atoi(yytext); return NUMBER;}
[+-]?([0-9]*[.])?[0-9]+          {yylval.number = atof(yytext); return DOUBLE_NUM;}
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