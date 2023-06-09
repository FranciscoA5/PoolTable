#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

//Vertex Shader source code
//não é c++ , é opengl shading language (GLSL)
const char* vertexShaderSource = "#version 330 core\n" //versao do glsl a usar
"layout (location = 0) in vec3 aPos;\n" //o layout ajuda o opengl com o veretx data que recebe , neste caso no layout 0 existe um vector para posicao
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 proj;\n"
"void main()\n"
"{\n"
"   gl_Position = proj * view * model * vec4(aPos, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" //outputs vec4
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";



GLfloat ZOOM = 10.0f;




glm::vec2 clickPos;
glm::vec2 prevClickPos;
glm::vec3 rotationAngles(0.0f, 0.0f, 0.0f);
float objectScale = 1.0f;

void mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		clickPos = glm::vec2(xpos, ypos);
		prevClickPos = clickPos;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		rotationAngles.x = 0.0f;
		rotationAngles.y = 0.0f;
	}
}

void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		prevClickPos = clickPos;
		clickPos = glm::vec2(xpos, ypos);

		glm::vec2 clickDelta = clickPos - prevClickPos;
		const float sensitivity = 0.2f;

		rotationAngles.y += clickDelta.x * sensitivity;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// Se faz zoom in
	if (yoffset == 1) {
		// Incremento no zoom, varia com a distância da câmara
		ZOOM += fabs(ZOOM) * 0.1f;
	}
	// Senão, se faz zoom out
	else if (yoffset == -1) {
		// Incremento no zoom, varia com a distância da câmara
		ZOOM -= fabs(ZOOM) * 0.1f;
	}
	
}

int main(void) {

    glfwInit(); //Inicializar glfw para criar uma janela
    
    //glfw nao sabe que versao do opengl estamos a usar , temos de dizer isso atraves de hints
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //especificamos que vamos usar a versao do opengl3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);//Major = 3 , Minor = 3 , porque é versao 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//especificar que opengl profile queremos usar , é como se fosse um package de funções , core profile dá-nos as funções modernas


	GLfloat vertices[] = {
		// Frente
		-0.9f, -0.05f, 0.45f,    // V0
		0.9f, -0.05f, 0.45f,     // V1
		0.9f, 0.05f, 0.45f,      // V2
		-0.9f, 0.05f, 0.45f,     // V3

		// Trás
		-0.9f, -0.05f, -0.45f,   // V4
		-0.9f, 0.05f, -0.45f,    // V5
		0.9f, 0.05f, -0.45f,     // V6
		0.9f, -0.05f, -0.45f,    // V7

		// Direita
		0.9f, -0.05f, 0.45f,     // V8
		0.9f, -0.05f, -0.45f,    // V9
		0.9f, 0.05f, -0.45f,     // V10
		0.9f, 0.05f, 0.45f,      // V11

		// Esquerda
		-0.9f, -0.05f, 0.45f,    // V12
		-0.9f, 0.05f, 0.45f,     // V13
		-0.9f, 0.05f, -0.45f,    // V14
		-0.9f, -0.05f, -0.45f,   // V15

		// Cima
		-0.9f, 0.05f, 0.45f,     // V16
		0.9f, 0.05f, 0.45f,      // V17
		0.9f, 0.05f, -0.45f,     // V18
		-0.9f, 0.05f, -0.45f,    // V19

		// Baixo
		-0.9f, -0.05f, 0.45f,    // V20
		-0.9f, -0.05f, -0.45f,   // V21
		0.9f, -0.05f, -0.45f,    // V22
		0.9f, -0.05f, 0.45f      // V23
	};

	GLuint indices[] = {
		// Face frontal
		0, 1, 2,
		2, 3, 0,

		// Face traseira
		4, 5, 6,
		6, 7, 4,

		// Face direita
		8, 9, 10,
		10, 11, 8,

		// Face esquerda
		12, 13, 14,
		14, 15, 12,

		// Face superior
		16, 17, 18,
		18, 19, 16,

		// Face inferior
		20, 21, 22,
		22, 23, 20
	};

    GLFWwindow* window = glfwCreateWindow(800, 800, "PoolTable", NULL, NULL);//width,height,name of the window , full screnn or not , last thing not important

    if (window == NULL) {

        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);//temos de dizer que queremos usar a janela

    // Inicia o gestor de extensões GLEW
    glewExperimental = GL_TRUE;
    glewInit();

	glfwSetScrollCallback(window, scrollCallback);
	
	

	glfwSetMouseButtonCallback(window, mouseClickCallback);
	glfwSetCursorPosCallback(window, mouseMovementCallback);
	
	



    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//dizemos ao Opengl para limpar a cor do buffer e dar-lhe outra
    glClear(GL_COLOR_BUFFER_BIT);//especificamos que queremos usar o comando no buffer de cor

    //temos um back buffer com a cor que queremos e um front buffer com a cor default , temos de trocar
    glfwSwapBuffers(window);


    glViewport(0, 0, 800, 800);//dizer que area da janela queremos que o OpenGl faça render

	//unsigned int no OPENGL
	//shader é um objeto do opengl , só podemos aceder por referencia
	//vertex shader pega nas posicoes do vertice e transforma (ou não) depende do que quisermos
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);//especificar que shader queremos criar

	//temos de dar ao vertex shader o source code (esta em cima da funcao main)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//GPU nao entende o source code , por isso temos de compilar em codigo maquina
	glCompileShader(vertexShader);

	//fazer o mesmo para o fragment shader , fragment shader atribui a cor aos pixeis
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//para usar os shaders temos de um meter num shader program
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//VBO é um array de referencias ,mas como so temos um objeto so precisamos de usar um 
	GLuint VAO, VBO, EBO;

	//opengl nao sabe onde encontrar o VBO, por isso criamos VAO
	//VAO guarda pointers de um ou mais VBO e diz ao OpenGL como interpretar
	//VAO permite trocar rapidamente entre varios VBO
	//criar o VAO sempre primeiro
	glGenVertexArrays(1, &VAO);

	//metemos 1 porque so temos um objeto
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Make the VAO the current VERTEX array object  by binding it
	glBindVertexArray(VAO);
	//bind the VBO specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//guardar os vertices no VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//tipo de buffer , o tamanho , os dados , uso dos dados

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//configurar o VAO para o Opengl saber como ler o VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//o 3 especifica o numero de componentes por atributo, cada vertice tem 3 posicoes(x,y,z)

	//para usar o VAO
	glEnableVertexAttribArray(0);//dar-lhe 0 porque é a posciao do vertix attribute

	//não obrigatorio mas para garantir que nao mudamos o VAO e VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//fazer isto depois do vao , porque o ebo esta guardao no vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Variables that help the rotation of the pyramid
	// Variables that help the rotation of the pyramid
	
	glEnable(GL_DEPTH_TEST);
	//uso dos dados
	//GL_Static vertices modify once and used many many times
	//GL_Stream vertices modify once and used a few times
	//GL_dynamic vertices modify multiple times and used many many times 


		//inicializar a matriz modelação,visualização,projeção
		//move o mundo inteiro
	
	while (!glfwWindowShouldClose(window)) {


		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//dizemos ao Opengl para limpar a cor do buffer e dar-lhe outra
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Tell opengl which Shader program to use
		glUseProgram(shaderProgram);

		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		
		// Assigns different transformations to each matrix
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -ZOOM));
		proj = glm::perspective(glm::radians(45.0f), (float)800 / 800, 0.1f, 100.0f);
		//rodar o objeto de acordo com o clique do rato
		model = glm::rotate(model, glm::radians(rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		
		  

		

		
		// Outputs the matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));




		//Bind the vao so OPengl knows to use it
		glBindVertexArray(VAO);
		 //glDrawArrays(GL_QUADS, 0, 24);
		 //glDrawArrays(GL_TRIANGLES,0,24);//como desenhar , o indice de inicio , quantidade de vertices a desenhar
		//glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);//como desenhar , quantos indices desenhar , o tipo dos indices e o index dos indices
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		//dizer ao glfw para procesar todos os eventos como a janela aparecer , mudar de tamanho , input etc , senão a janela fica num estado sem resposta
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}
  
