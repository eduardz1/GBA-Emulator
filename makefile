CC=clang
CFLAGS=

# Dependencies
OBJS=bus.o memory.o master.o arm7tdmi.o

all: $(OBJS)

%.o: src/%.cpp
	$(CC) -c $(CFLAGS) src/*.cpp

clean:
	rm -f *.o master users nodes log.txt ledger.txt *~

run: all
	./master