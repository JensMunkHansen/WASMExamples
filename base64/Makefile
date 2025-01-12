# Compiler and tools
EMCC = emcc
WASM2WAT = wasm2wat
WAT2WASM = wat2wasm

# Files
SRC = base64c.c
JS_OUT = base64c.js
WASM_OUT = base64c.wasm
WAT_OUT = base64c.wat

OPT_WAT_IN = base64.wat
OPT_WASM_OUT = base64.wasm

# Default target
all: $(WASM_OUT) $(WAT_OUT) $(OPT_WASM_OUT)

# Compile C to WebAssembly and JS
$(WASM_OUT) $(JS_OUT): $(SRC)
	$(EMCC) $(SRC) -o $(JS_OUT)

# Convert WebAssembly binary to WebAssembly text
$(WAT_OUT): $(WASM_OUT)
	$(WASM2WAT) $(WASM_OUT) > $(WAT_OUT)

$(OPT_WASM_OUT) : $(OPT_WAT_IN)
	$(WAT2WASM) $(OPT_WAT_IN) -o $(OPT_WASM_OUT)
# Clean generated files
clean:
	rm -f $(JS_OUT) $(WASM_OUT) $(WAT_OUT) $(OPT_WASM_OUT)
base64c: base64c.js
	emcc base64c.c -o base64c.js
	wasm2wat base64c.wasm > base64c.wat

