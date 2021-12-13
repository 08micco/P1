CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

LDFLAGS 	= structs.h Prototyper.h
OBJFILES 	= CO2-program.o Tips.o User_profile.o printing.o jason-functions.o comapre.o Charts.o calculation.o tiny-json/tiny-json.o
TARGET		= CO2-program.exe

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~


