%{
#include <stdio.h>
#include <string.h>
#include "template.tab.h"
#define TT_TOK(a) return a
#define STR yylval.string = (char *) strdup(yytext)
%}
%s data
%s command
%%

<data>\\\[%	yylval.string="[%"; TT_TOK(DATA);
<*>\[%		BEGIN(command); TT_TOK(COMMAND);
<data>([^\[]|\n)+	STR; TT_TOK(DATA); 
<data>\[	STR; TT_TOK(DATA);

                 /* parse inside directives [% ... %] */
<command>{
 END                  yylval.string=""; TT_TOK(END_TOK);
 [A-Z]+		      STR; TT_TOK(DIRECTIVE); /* possible command */
 [^[:space:]|]+        STR; TT_TOK(IDENTIFIER);
 [[:space:]]+	      /* nothing */;
 \|                   TT_TOK(FILTER_OP);
 %]\n?		      BEGIN(data);
}
%%



