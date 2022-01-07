PREFIX=/usr/local

install:
	gcc -o shell main.c command.c
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/lib
	cp shell $(PREFIX)/bin
remove:
	rm $(PREFIX)/bin/shell
