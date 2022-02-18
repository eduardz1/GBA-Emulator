CC=g++
CFLAGS=

# Dependencies
OBJS=master.o arm7tdmi.o bus.o

all: $(OBJS)
	$(CC) $(CFLAGS) *.o -o master

%.o: src/%.cpp
	$(CC) -c $(CFLAGS) src/*.cpp

clean:
	rm -f *.o master *.dump log.txt  *~

run: all
	./master