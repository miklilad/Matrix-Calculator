COMP=g++
FLAGS=-Wall -pedantic -std=c++14 -g
NAME=Calculator

all: compile doc

compile: main.o Parser.o Calculator.o Matrix.o DenseMatrix.o SparseMatrix.o
	$(COMP) $(FLAGS) $^ -o $(NAME)

compile2: main.o Parser.o Calculator.o Matrix.o DenseMatrix.o SparseMatrix.o
	$(COMP) $(FLAGS) $^ -fsanitize=address -o $(NAME)

%.o: ./src/%.cpp
	$(COMP) $(FLAGS) -c $<

clean:
	rm -rf *.o ./doc $(NAME)

run: compile2
	./$(NAME)

valgrind: compile
	valgrind --track-origins=yes ./$(NAME)

count:
	wc -l ./src/*

doc: ./src/Parser.h ./src/Parser.cpp ./src/Calculator.h ./src/Calculator.cpp ./src/Matrix.h ./src/Matrix.cpp ./src/DenseMatrix.h ./src/DenseMatrix.cpp ./src/SparseMatrix.h ./src/SparseMatrix.cpp ./src/main.cpp
	doxygen configure

debug:	compile
	gdb -tui ./$(NAME)

ex: compile2
	./$(NAME) < examples/input01.txt
