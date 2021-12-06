
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

src = $(wildcard *.c)
src += $(wildcard ../*.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d) 

.PHONY: build all clean

build: json-parse.exe

all: clean build

clean::
	rm -rf $(dep)
	rm -rf $(obj)
	rm -rf *.exe

	
json-parse.exe: json-parse.o tiny-json/tiny-json.o
	gcc $(CFLAGS) -o $@ $^

	

-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@