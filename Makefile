CPP      = g++
OBJ      = Game.o BoolMatrix.o SentenceMem.o
BIN      = Game.exe

.PHONY: all all-before all-after

all: all-before $(BIN) all-after

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $@

Game.o: Game.cpp Game.h BoolMatrix.h SentenceMem.h
	$(CPP) -c $< -o $@

BoolMatrix.o: BoolMatrix.cpp BoolMatrix.h Game.h
	$(CPP) -c $< -o $@

SentenceMem.o: SentenceMem.cpp SentenceMem.h Game.h
	$(CPP) -c $< -o $@
