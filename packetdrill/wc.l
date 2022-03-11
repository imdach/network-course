%{
  int chars = 0;
  int words = 0;
  int lines =0;

%}

%%
[a-zA-Z]+ {
		words++;
		chars += strlen(yytext);
          }

\n        {     chars++; lines++;}

.         {     chars++;     }

%%


int main(int args, char **argv)
{
	yylex();
	printf("lines=%d words=%d chars=%d\n", lines, words, chars);
	return 0;
}