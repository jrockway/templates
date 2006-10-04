%{
#include <stdio.h>
#include <string.h>
#include "template.tab.h"
#define TOK(a) return a
#define STR yylval.string = strdup(yytext);
%}     

%option   8bit 
%x data
%x command
%x comment
%x qq_string
%x q_string
%%

<data,INITIAL>{
 \[%#			 BEGIN(comment);
 \[%			 BEGIN(command);
 \\\[%			 yylval.string="[%"; TOK(DATA);
 ([^\\\[]|\n)+		 STR; TOK(DATA);  /* everything */
 \\[^[]?[^%]?		 STR; TOK(DATA); /* don't eat all \s */
}
                 /* parse inside directives [% ... %] */
<command>{
 END			 TOK(END);
			 /* directives that end w/ [% END %] */
 IF		      	 TOK(IF);
 SET			 TOK(SET);
 ELSE		      	 TOK(ELSE);
 ELSIF	      	 	 TOK(ELSIF);
 LOOP		      	 TOK(LOOP);
 WRAP		      	 TOK(WRAP);

	 /* operators */
 =                       TOK(ASSIGN);

 IS			 TOK(EQ);
 ==                      TOK(EQ);
 AND			 TOK(AND);
 OR			 TOK(OR);
 &&			 TOK(AND);
 \|\|			 TOK(OR);
 \+\+			 TOK(INC);
 --			 TOK(DEC);
 \+			 TOK(ADD);
 -			 TOK(SUB);
 \<=			 TOK(LTEQ);
 \>=   			 TOK(GTEQ);
 \<			 TOK(LT);
 \>			 TOK(GT);

        /* strings */
 ["]			 BEGIN(qq_string); 
 [']                     BEGIN(q_string);

	/* other directives */
 [A-Z]+			 STR; TOK(DIRECTIVE);
 [^[:space:]|\]\[%]+  	 STR; TOK(IDENTIFIER);
 [[:space:]]+	      /* nothing */;
 \|                   	 TOK(FILTER_OP);
 %]\n? 		      	 BEGIN(data); TOK(E);
}

<q_string>[^"]+		 STR; TOK(STRING);
<q_string>[^\]["] 	 BEGIN(command);
<qq_string>[^']+	 STR; TOK(STRING);
<qq_string>[^\]['] 	 BEGIN(command);


<comment>[^%]*	      /* ignore */;
<comment>%]\n?        BEGIN(data);
