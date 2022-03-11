%option noyywrap

%{

	void yyerror(char*);
	#include "calc.tab.h"
%}

%%

     /* a-z为变量 */
[a-z]	{
            yylval = *yytext - 'a';
            return VARIABLE;
    	}

    /* 整数 */
[0-9]+	{
            yylval = atoi(yytext);
            return INTEGER;
    	}

    /* 运算符 */
[-+()=/*\n]	{return *yytext;}

    /* 空白被忽略 */
[ \t]    ;

    /* 其他字符都是非法的 */
.    yyerror("invalid input");

%%