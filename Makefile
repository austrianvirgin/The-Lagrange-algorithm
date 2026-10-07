CC = g++
TARGET = approximator
OBJS = main.o Approximator.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lm
main.o: main.cpp Approximator.h
	$(CC) -c main.cpp
Approximator.o: Approximator.cpp Approximator.h
	$(CC) -c Approximator.cpp

clean:
	rm -f $(OBJS) $(TARGET)
