RENDERER_SOURCE = src/Maths.cpp src/Rasterizer.cpp src/Renderer.cpp

main: apps/main.cpp $(RENDERER_SOURCE)
	g++ apps/main.cpp $(RENDERER_SOURCE) -g -I /d/MinGW_dev/include/ -I ./includes -O2 -lgdi32 -o bin/main.exe

circle: apps/circle.cpp $(RENDERER_SOURCE)
	g++ apps/circle.cpp $(RENDERER_SOURCE) -g -I /d/MinGW_dev/include/ -I ./includes -O2 -lgdi32 -o bin/circle.exe