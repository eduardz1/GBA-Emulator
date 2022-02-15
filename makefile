CC=g++
CFLAGS=

# Dependencies
OBJS= memory.o master.o arm7tdmi.o

all: $(OBJS)
	$(CC) $(CFLAGS) *.o -o master

%.o: src/%.cpp
	$(CC) -c $(CFLAGS) src/*.cpp

clean:
	rm -f *.o master users nodes log.txt ledger.txt *~

run: all
	./master