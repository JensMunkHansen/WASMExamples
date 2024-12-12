all:
	g++ -c factory_and_base.cpp -o factory_and_base.o && ar rcs libfactory_and_base.a factory_and_base.o
	g++ -c derived.cpp -o derived.o && ar rcs libderived.a derived.o
	#g++ main.cpp -L. -lderived -lfactory_and_base -o program # Not working
	g++ main.cpp -L. -lfactory_and_base -lderived -o program # First resolves for the latter

# it resolves symbols on a "first come, first serve" basis and does not revisit libraries already processed
# Linux/Win: link error
# WASM: runtime error

# Embind works by dynamically binding C++ classes and functions to
# JavaScript objects. Unlike static linking, where the linker resolves
# symbols at compile-time, Embind defers this resolution to
# runtime. If a class or function is not properly registered or
# exposed, you will encounter runtime errors such as:

# Runtime errors occur instead of link errors because:

# Embind defers type and symbol resolution to runtime.
#WASM relies on dynamic exports and imports via JavaScript glue code.
#The factory and derived class registration may rely on runtime initialization that fails if not properly linked or exposed.

#To mitigate these issues:

# Carefully manage Embind bindings.
# Use correct link order or --start-group/--end-group.
# Enable runtime assertions for better error messages during development.
