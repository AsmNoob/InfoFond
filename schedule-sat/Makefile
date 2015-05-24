build/schedpb: src/lex.yy.c src/parser.cpp src/SchedSpec.cpp
	cc -c src/lex.yy.c -o src/lex.yy.o
	c++ src/lex.yy.o src/parser.cpp src/SchedSpec.cpp -o build/schedpb -lminisat -DYYDEBUG=1
src/lex.yy.c: src/parser.cpp
	lex -o src/lex.yy.c src/parser.l 
src/parser.cpp: src/parser.y
	yacc -d -o src/parser.cpp src/parser.y
clean:
	rm -f src/lex.yy.c
	rm -f src/parser.cpp
	rm -f src/lex.yy.o
	rm -f build/schedpb
	rm -f src/parser.h
