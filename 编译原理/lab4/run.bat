flex lex.l
bison -d parser.y	
gcc -g -o parser ast.c parser.tab.c lex.yy.c display.c object.c