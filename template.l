%{
#include <stdio.h>
#include <string.h>
#include "template.tab.h"
#define TT_TOK(a) return a
#define STR yylval.string = strdup(yytext);
%}     

%option   8bit 
%x data
%x command
%x comment
%%

<data,INITIAL>{
 \[%#			 BEGIN(comment);
 \[%			 BEGIN(command);
 \\\[%			 yylval.string="[%"; TT_TOK(DATA);
 ([^\\\[]|\n)+		 STR; TT_TOK(DATA);  /* everything */
 \\[^[]?[^%]?		 STR; TT_TOK(DATA); /* don't eat all \s */
}
                 /* parse inside directives [% ... %] */
<command>{
 END			 TT_TOK(END);
			       /* directives that end w/ [% END %] */
 IF		      	 TT_TOK(IF);
 ELSE		      	 TT_TOK(ELSE);
 ELSIF	      	 	 TT_TOK(ELSIF);
 LOOP		      	 TT_TOK(LOOP);
 WRAP		      	 STR; TT_TOK(BDIRECTIVE);
 			      /* other directives */
 [A-Z]+			 STR; TT_TOK(DIRECTIVE);
 [<>=] 		      	 STR; TT_TOK(OPERATOR);
 [^[:space:]|\]\[%]+  	 STR; TT_TOK(IDENTIFIER);
 [[:space:]]+	      /* nothing */;
 \|                   	 TT_TOK(FILTER_OP);
 %]\n? 		      	 BEGIN(data); TT_TOK(E);
}

<comment>[^%]*	      /* ignore */;
<comment>%]\n?        BEGIN(data);
