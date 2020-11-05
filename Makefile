CC = g++
CCFLAGS = -g  -std=c++11
A = -lncursesw

SnakeGame: Main.o SnakeGame.o Point.o
	$(CC) $(CCFLAGS) -o SnakeGame Main.o SnakeGame.o Point.o $(A)

clean:
	rm -f *.o

%.o : %.cpp %.h
	$(CC) $(CCFLAGS) -c $<  $(A)

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<  $(A)

% : %.cpp
	$(CC) $(CCFLAGS) -o $@ $< $(A)
