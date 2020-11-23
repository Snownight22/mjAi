CC = gcc
DEBUG = -g
INC = -I./include/ 
LIBS = -lm
CFLAGS = -std=gnu11
#-D__FILENAME__='"$(subst $(dir $<), ,$<)"'
SHARED = -fpic -shared


SRC_DIR = .

SRC = $(foreach dir,$(SRC_DIR), $(wildcard $(dir)/*.c))

BIN = mj

SHAREDBIN = mj.so

all:$(SRC)
	$(CC) $(SRC) $(CFLAGS) $(DEBUG) $(INC) $(LIBS) -o $(BIN)

testdebug:$(SRC)
	$(CC) $(SRC) $(CFLAGS) $(DEBUG) $(INC) $(LIBS) -DTEST_DEBUG -o $(BIN)

shared:$(SRC)
	$(CC) $(SRC) $(CFLAGS) $(INC) $(LIBS) $(SHARED) -o $(SHAREDBIN)


.PHONY:

clean:
	rm -rf $(BIN) $(SHAREDBIN)
