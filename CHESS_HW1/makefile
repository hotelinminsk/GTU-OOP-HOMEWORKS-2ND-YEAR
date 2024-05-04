all: clean compile run

compile: main.cpp board.cpp piece.cpp
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ -O3 -o test main.cpp board.cpp piece.cpp -lm

run:
	@echo "-------------------------------------------"
	@echo "Running the game...."
	@echo "======================================================================="
	@./test
	@echo "======================================================================="
	@echo "Exiting the program...."

clean:
	@echo "-------------------------------------------"
	@echo "Cleaning..."
	@rm -f *.o
	@rm -f test