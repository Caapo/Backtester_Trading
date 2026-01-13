#MEMO : 7 : CDE SRCOBJS R COCEXE CCCO

#C
CC = g++
FLAGS = -Iinclude -Wall

#D
INC = include
SRC = src
BLD = build
BIN = bin

#E
MAIN = $(BIN)/main

#SRC & OBJS
SRCS := $(shell find $(SRC) -type f -name '*.cpp')
OBJS := $(SRCS:$(SRC)/%.cpp=$(BLD)/%.o)


#R

all : $(MAIN)

help : 
	@echo "Commandes disponibles :"
	@echo "  make help          -> Affiche le manuel"
	@echo "  make / make all    -> compile le programme principal ($(MAIN))"
	@echo "  make clean     -> supprime .o (BUILD) et .exe (BIN)"



clean : 
	rm -rf $(BLD) $(BIN)

.PHONY : all help clean


#cO => cEXE
$(MAIN) : $(OBJS)
	mkdir -p $(BIN)
	$(CC) $(FLAGS) $(OBJS) -o $(MAIN)


#--------- cC => cO ---------

#Tous les .cpp en .o :
$(BLD)/%.o : $(SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@


#.cpp en .o individuellement
$(BLD)/MarketData.o : $(INC)/core/MarketData.hpp $(SRC)/core/MarketData.cpp 
	mkdir -p $(BLD)
	$(CC) $(FLAGS) -c  $(SRC)/core/MarketData.cpp -o $(BLD)/core/MarketData.o
	

$(BLD)/Candle.o : $(INC)/core/Candle.hpp $(SRC)/core/Candle.cpp 
	mkdir -p $(BLD)
	$(CC) $(FLAGS) -c  $(SRC)/core/Candle.cpp -o $(BLD)/core/Candle.o
	

