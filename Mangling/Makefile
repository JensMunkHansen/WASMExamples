# Compiler and tools
EMCC := emcc
AR := emar
NM := llvm-nm

# Source and output files
SRC := mylib.cpp
OBJ := mylib.o
LIB := libmylib.a
EXPORTED_FUNCTIONS_JSON := exported_functions.json

# Compiler flags
CFLAGS := -c
ARFLAGS := rcs

# Target to build the archive
$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

# Target to compile the object file
$(OBJ): $(SRC)
	$(EMCC) $(CFLAGS) $< -o $@

# Extract mangled names and format them as EXPORTED_FUNCTIONS
$(EXPORTED_FUNCTIONS_JSON): $(LIB)
	$(NM) $(LIB) | grep ' T ' | awk '{print $$3}' | sed "s/^/'_/; s/$$/'/" | paste -sd "," | sed "s/^/[/; s/$$/]/" > $@

# Target to build the final WebAssembly module
output.js: $(LIB) $(EXPORTED_FUNCTIONS_JSON)
	$(EMCC) -o $@ $(LIB) -sSINGLE_FILE=1 -sENVIRONMENT='node' -sMODULARIZE=1 -sEXPORTED_FUNCTIONS=$(shell cat $(EXPORTED_FUNCTIONS_JSON))

all:	output.js
# Clean
.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB) output.js output.wasm $(EXPORTED_FUNCTIONS_JSON)
