CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

dep = $(obj:.o=.d) 

LDFLAGS 	= 
OBJFILES 	= main_program.o tips.o initialize_user_profile.o printing.o json_functions.o compare.o charts.o calculation.o tiny-json/tiny-json.o
TARGET		= main_program.exe

all: $(TARGET) data_simulator.exe

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~ *.exe $(dep) *.o *.d

data_simulator.exe: data_simulator.o
	gcc $(CFLAGS) -o $@ $^

	
-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@