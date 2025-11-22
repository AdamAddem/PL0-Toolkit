all:
	gcc lex.c -o lex && gcc parsercodegen_complete.c -o pcg && gcc vm.c -o vm && ./lex program.txt && ./pcg && ./vm

run:
	./lex input.txt && ./pcg && ./vm

clean:
	rm lex pcg vm token_list.txt elf.txt