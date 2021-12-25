PREFIX=/usr/local

install:
	gcc -o shell main.c
	gcc -o cls cls.c
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/lib
	cp shell $(PREFIX)/bin
	cp cls $(PREFIX)/bin
remove:
	rm $(PREFIX)/bin/shell
	rm $(PREFIX)/bin/cls
