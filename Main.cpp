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
		glfwTerminate();//Fecha a library glfw
		return -1;
	}

	glfwMakeContextCurrent(window);//Tornar o contexto da janela na thread atual

	while (!glfwWindowShouldClose(window)) {//Espera pelo pedido de fecho da janela(Alt-F4 ou bot�o)
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);//realizada apos a execucao de todos os commandos de renderiza�ao de frames

		//realizada apenas na thread principal, processa eventos que ja tenham sido recebidos, feito apos swapbuffers e � mais para jogos
		glfwPollEvents();
	}

	glfwDestroyWindow(window);//Destruir imediatamente uma janela

	glfwTerminate();//Fecha a library glfw
	return 0;
}