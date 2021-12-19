CC=gcc
OBJS=matamikya.o matamikya_main.o matamikya_tests.o amount_set_str.o matamikya_print.o
EXEC=matamikya
CFLAGS=-std=c99 -Wall -Werror -pedantic-errors
ARCHIVE_FLAGS=-L. -lmtm -lm -las

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(ARCHIVE_FLAGS) -o $@

matamikya_main.o:
	$(CC) -c $(CFLAGS) tests/matamikya_main.c

matamikya_tests.o:
	$(CC) -c $(CFLAGS) tests/matamikya_tests.c

clean:
	rm -rf $(OBJS) $(EXEC)