all:
	# Native version
	clang example.cpp -o example -lSDL2 -lSDL2_gfx
	# Web version
	emcc --bind example.cpp -o example.html -s USE_SDL=2 -s USE_SDL_GFX=2
