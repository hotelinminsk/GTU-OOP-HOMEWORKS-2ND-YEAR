CC = g++
CFLAGS = -std=c++11 -Wall -O3
SRC_DIR = sources
OBJ_DIR = obj
BIN_DIR = bin
EXECUTABLE = test

# Get the list of source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Generate the list of object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: clean compile run

compile: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./bin/$(EXECUTABLE)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
