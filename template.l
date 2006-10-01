%{
#include <stdio.h>
#include <string.h>
#include "template.tab.h"
#define TT_TOK(a) return a
#define STR yylval.string = (char *) strdup(yytext)
%}
%x data
%x command
%%
	BEGIN(data);

<data>\[%	BEGIN(command);
<data>\\\[%	yylval.string="[%"; TT_TOK(DATA);		
<data>([^\[]|\n)+	STR; TT_TOK(DATA); 
<data>\[	STR; REJECT; TT_TOK(DATA);

                 /* parse inside directives [% ... %] */
<command>{
 END             yylval.string=""; TT_TOK(END_TOK);
 [A-Z]+		 STR; TT_TOK(DIRECTIVE); /* possible command */
 [A-Za-z0-9_]+   STR; TT_TOK(IDENTIFIER);
 [[:space:]]+	 /* nothing */;
 \|              yylval.string=""; TT_TOK(FILTER_OP);
 %\]		 yylval.string=""; BEGIN(data);
}
%%



