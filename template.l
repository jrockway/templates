%{
#include <stdio.h>
#include <string.h>
#include "template.tab.h"
#define TT_TOK(a) return a
#define STR yylval.string = (char *) strdup(yytext)
%}
%x data
%x command
%x comment
%%

<data,INITIAL>{
 \[%#		BEGIN(comment);
 \[%		BEGIN(command);

 \\\[%	        yylval.string="[%"; TT_TOK(DATA);
 ([^\\\[]|\n)+	STR; TT_TOK(DATA); 
 \[[^%]		STR; TT_TOK(DATA);
}
                 /* parse inside directives [% ... %] */
<command>{
 END                  yylval.string=""; TT_TOK(END_TOK);
 /* directives that have a following [% END %] */
 IF		      STR; TT_TOK(BDIRECTIVE);
 ELSE		      STR; TT_TOK(BDIRECTIVE);
 ELSIF		      STR; TT_TOK(BDIRECTIVE);
 LOOP		      STR; TT_TOK(BDIRECTIVE);
 WRAP		      STR; TT_TOK(BDIRECTIVE);
 /* other directives */
 [A-Z]+		      STR; TT_TOK(DIRECTIVE);
 [^[:space:]|\]\[%]+  STR; TT_TOK(IDENTIFIER);
 [[:space:]]+	      /* nothing */;
 \|                   TT_TOK(FILTER_OP);
}
<comment,command>%]\n? BEGIN(data); TT_TOK(E);

<comment>[^%]*	      /* ignore */;
%%



