main: main.cpp Maths.cpp Rasterizer.cpp Renderer.cpp
	g++ main.cpp Maths.cpp Rasterizer.cpp Renderer.cpp -g -I /d/MinGW_dev/include/ -O2 -lgdi32