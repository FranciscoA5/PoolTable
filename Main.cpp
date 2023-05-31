#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <GL/glu.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void drawCube()
{
    GLfloat vertices[] =
    {
        -8, -3, -1,   -8, -3,  1,   -8,  3,  1,   -8,  3, -1,
        8, -3, -1,    8, -3,  1,    8,  3,  1,    8,  3, -1,
        -8, -3, -1,   -8, -3,  1,    8, -3,  1,    8, -3, -1,
        -8,  3, -1,   -8,  3,  1,    8,  3,  1,    8,  3, -1,
        -8, -3, -1,   -8,  3, -1,    8,  3, -1,    8, 3, -1,
        -8, -3,  1,   -8,  3,  1,    8,  3,  1,    8, -3,  1
    };

    GLfloat colors[] =
    {
        0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
        1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
        0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
        0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
        0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
        0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
    };

    static float alpha = 0;
    //attempt to rotate cube
    glRotatef(150, 1, 0, 0);
    glRotatef(30, 0, 1, 0);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    alpha += 1;
}


float cameraDistance = 5.0f;  // Distancia inicial da camara
const float zoomSpeed = 0.1f; // velocidade do zoom


float rotationAngleX = 0.0f;  // Initial rotation angle around X-axis
float rotationAngleY = 0.0f;  // Initial rotation angle around Y-axis
float rotationAngleZ = 0.0f;  // Initial rotation angle around Z-axis
double lastMouseX = 0.0;
double lastMouseY = 0.0;
double lastMouseZ = 0.0;
bool isMouseDragging = false;

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

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetScrollCallback(window, scrollCallback);

	while (!glfwWindowShouldClose(window)) {//Espera pelo pedido de fecho da janela(Alt-F4 ou botão)


        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective(150, 800.0/ 600.0, 1, 150);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0, 0, -cameraDistance);
        glRotatef(rotationAngleX, 1, 0, 0);
        glRotatef(rotationAngleY, 0, 1, 0);
        glRotatef(rotationAngleZ, 0, 0, 1);
        drawCube();
		glfwSwapBuffers(window);//realizada apos a execucao de todos os commandos de renderizaçao de frames
        
		//realizada apenas na thread principal, processa eventos que ja tenham sido recebidos, feito apos swapbuffers e é mais para jogos
		glfwPollEvents();
	}

	glfwDestroyWindow(window);//Destruir imediatamente uma janela

	glfwTerminate();//Fecha a library glfw
	return 0;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isMouseDragging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        else if (action == GLFW_RELEASE) {
            isMouseDragging = false;
        }
    }
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isMouseDragging) {
        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;
        double deltaZ = ypos - lastMouseY;
        rotationAngleX += static_cast<float>(deltaY) * 0.1f;
        rotationAngleY += static_cast<float>(deltaX) * 0.1f;
        rotationAngleZ += static_cast<float>(deltaZ) * 0.1f;
        lastMouseX = xpos;
        lastMouseY = ypos;
        lastMouseZ = ypos;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraDistance -= yoffset * zoomSpeed;
    if (cameraDistance < 1.0f) {
        cameraDistance = 1.0f;
    }
}