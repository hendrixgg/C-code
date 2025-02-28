.PHONY: all

all: quine.c quine.out.c
	sha256sum quine.c quine.out.c

quine.out.c: quine.c
	gcc -Wall -Wextra -o quine quine.c
	./quine > quine.out.c

quine.c: template.c put_in_first_empty_double_quotes.exe
	./escape.exe < template.c | ./put_in_first_empty_double_quotes.exe template.c > quine.c

put_in_first_empty_double_quotes.exe: put_in_first_empty_double_quotes.c
	gcc -Wall -Wextra -o put_in_first_empty_double_quotes.exe put_in_first_empty_double_quotes.c
