# Compiler
COMP := cpp_compiler/bin/g++

# Compiler & Linker Flags
CXXFLAGS := -std=c++23 -Isrc  -Wno-narrowing -Wno-enum-compare
LDFLAGS := -O2 -Lsrc -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

# Source Files
SRC_DIR := src
SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.cpp")

# Generate object file names (same directory as .cpp but with .o extension)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

# Executable Name
TARGET := Shinobi_Chronicles.exe

# Default target
all: $(TARGET)

# Compile each .cpp file into a .o file (inside the same folder)
%.o: %.cpp
	$(COMP) -c $(CXXFLAGS) $< -o $@

# Link all .o files into the final executable
$(TARGET): $(OBJ_FILES)
	$(COMP) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Clean compiled files
clean:
	@find $(SRC_DIR) -type f -name "*.o" -delete
	@rm -f $(TARGET)

cl :
	@rm -f $(TARGET)
