INC_DIR = 	./include
OBJS    =	BF.o  files.o  HT.o  murmur3.o  prompt.o  RBT.o  runelection.o  voter.o
SOURCE  =	./src/BF.c  ./src/files.c  ./src/HT.c  ./src/murmur3.c  ./src/prompt.c  ./src/RBT.c  ./src/runelection.c  ./src/voter.c
HEADER  =	./include/BF.h  ./include/files.h  ./include/HT.h  ./include/murmur3.h  ./include/prompt.h  ./include/RBT.h  ./include/runelection.h  ./include/voter.h
OUT     =	runelection
CC      =	gcc
FLAGS   =       -Wall   -g      -c  -std=c99 	-I$(INC_DIR)	-D_XOPEN_SOURCE=700

$(OUT): $(OBJS)
	$(CC)   -g  -lm    $(OBJS) -o    $@	

BF.o: ./src/BF.c
	$(CC)   $(FLAGS)        ./src/BF.c

files.o: ./src/files.c
	$(CC)   $(FLAGS)        ./src/files.c

HT.o: ./src/HT.c
	$(CC)   $(FLAGS)        ./src/HT.c

murmur3.o: ./src/murmur3.c
	$(CC)   $(FLAGS)        ./src/murmur3.c

prompt.o: ./src/prompt.c
	$(CC)   $(FLAGS)        ./src/prompt.c

RBT.o: ./src/RBT.c
	$(CC)   $(FLAGS)        ./src/RBT.c

runelection.o: ./src/runelection.c
	$(CC)   $(FLAGS)        ./src/runelection.c

voter.o: ./src/voter.c
	$(CC)   $(FLAGS)        ./src/voter.c

clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

