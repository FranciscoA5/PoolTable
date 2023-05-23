#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(800, 600, "PoolTable", nullptr, nullptr);
	//window = glfwCreateWindow(800, 600, "My 1st P3D Project", glfwGetPrimaryMonitor(), nullptr); // Full screen
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//faz com que a janela que está em argumento seja a que está ativa na thread.

	while (!glfwWindowShouldClose(window)) {//glfwWindowShouldClose(window)-pedido de fecho da janela, pode ser no Alt-F4 ou no ícone de fecho
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);//Destrói imediatamene a janela

	glfwTerminate();//Termina a biblioteca glfw
	return 0;
}
