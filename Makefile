# 1. Compiler and Flags
CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
CFLAGS   := -Iinclude
LDFLAGS  := -lpthread -ldl

# 2. Paths
SRC_DIR := src
LIB_DIR := lib
OBJ_DIR := build
BIN_DIR := bin

# 3. Target Name
TARGET := $(BIN_DIR)/ProgessManager

# 4. Source Files
# All your .cpp files in src/
CPP_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
# The sqlite3.c file in lib/
C_SOURCES   := $(LIB_DIR)/sqlite3.c

# 5. Object Files
# Convert .cpp and .c paths to .o paths in the build/ folder
OBJECTS := $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
           $(OBJ_DIR)/sqlite3.o

# 6. Default Rule
all: $(TARGET)

# 7. Link the Executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# 8. Compile C++ Source Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 9. Compile the SQLite C Source File
$(OBJ_DIR)/sqlite3.o: $(LIB_DIR)/sqlite3.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 10. Clean Rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean