CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic

txt2epub:
    @$(CC) $(CFLAGS) txt2epub.c -o txt2epub

tokenring:
	@$(CC) $(CFLAGS) tokenring.c -o tokenring

clean:
    @rm -rf txt2epub
    @rm -rf .epub
    @rm -rf .zip
	@rm -rf .fifo	