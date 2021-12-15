CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

dep = $(obj:.o=.d) 

LDFLAGS 	= 
OBJFILES 	= CO2-program.o Tips.o User_profile.o printing.o jason-functions.o compare.o Charts.o calculation.o tiny-json/tiny-json.o
TARGET		= CO2-program.exe

all: $(TARGET) power_simulation.exe

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~ *.exe $(dep) *.o *.d

power_simulation.exe: power_simulation.o
	gcc $(CFLAGS) -o $@ $^

	
-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@