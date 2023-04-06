%{
#include <stdio.h>
#include "y.tab.h"
%}
%%
"update"               {return UPDATE;}
"remove"               {return REMOVE;}
"create_element"        {return CREATE_EL;}
"create_schema"         {return CREATE_SCH;}
"true"|"false"          {return BOOL;}

[0-9]+                  {return NUMBER;}
[a-zA-Z][a-zA-Z0-9]*    {return WORD;}
"="                      {return EQUAL;}
"!="                     {return NOT_EQUAL;}
">"                      {return MORE;}
"<"                      {return LESS;}
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