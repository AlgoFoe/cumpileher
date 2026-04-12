all:
	gcc main.c lexer.c parser.c codegen.c symbol_table.c -o cumpileher

run:
	./cumpileher