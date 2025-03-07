PROGRAM = Output
Output: GLFWTest.o glad.o 
	g++ -o $(PROGRAM) GLFWTest.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl glad.o
GLFWTest.o: src/GLFWTest.cpp
	g++ -c -g src/GLFWTest.cpp
glad.o: dep/glad.c
	g++ -c dep/glad.c
.PHONY: clean run

clean: 
	-rm *.o
run:
	./$(PROGRAM)
