all:
	gcc lex.c -o lex && gcc parsercodegen.c -o pcg && gcc vm.c -o vm

run:
	./lex input.txt && ./pcg && ./vm

clean:
	rm lex pcg vm token_list.txt elf.txt