%{
#include <stdio.h>
#include <string.h>
#include "template.tab.h"
#define TT_TOK(a) return a
#define STR yylval = (YYSTYPE) strdup(yytext)
%}
%x command
%%

\\\[%			 TT_TOK(LIT_OPEN); /* \[% -> [% */

			 /* parse inside directives [% ... %] */
\[%		         BEGIN(command);
<command>END             TT_TOK(END_TOK);
<command>[A-Z]+		 STR; TT_TOK(DIRECTIVE); /* possible command */
<command>[^%[:space:]]+  STR; TT_TOK(IDENTIFIER);
<command>%\]		 BEGIN(0);

%%
