#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;
float zoomLevel = 1.0f;
float zoomSpeed = 0.1f;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool mousePressed = false;
glm::mat4 model = glm::mat4(1.0f);


const GLchar* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;

    uniform float zoomLevel;

    out vec3 vertexColor;

    void main()
    {
        vec4 transformedPosition = vec4(position * zoomLevel, 1.0);
        gl_Position = transformedPosition;
        vertexColor = color;
    }
)";

const GLchar* fragmentShaderSource = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(vertexColor, 1.0);
    }
)";

GLfloat vertices[] = {
    // Front face
    -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, // Bottom left
    0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 0.0f, // Bottom right
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f, // Top right
    -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f, // Top left

    // Back face
    -0.5f, -0.5f, -0.5f,    0.0f, 0.8f, 0.0f, // Bottom left
    0.5f, -0.5f, -0.5f,     0.0f, 0.8f, 0.0f, // Bottom right
    0.5f, 0.5f, -0.5f,      0.0f, 0.8f, 0.0f, // Top right
    -0.5f, 0.5f, -0.5f,     0.0f, 0.8f, 0.0f, // Top left

    // Left face
    -0.5f, -0.5f, -0.5f,    0.0f, 0.6f, 0.0f, // Bottom back
    -0.5f, -0.5f, 0.5f,     0.0f, 0.6f, 0.0f, // Bottom front
    -0.5f, 0.5f, 0.5f,      0.0f, 0.6f, 0.0f, // Top front
    -0.5f, 0.5f, -0.5f,     0.0f, 0.6f, 0.0f, // Top back

    // Right face
    0.5f, -0.5f, -0.5f,     0.0f, 0.4f, 0.0f, // Bottom back
    0.5f, -0.5f, 0.5f,      0.0f, 0.4f, 0.0f, // Bottom front
    0.5f, 0.5f, 0.5f,       0.0f, 0.4f, 0.0f, // Top front
    0.5f, 0.5f, -0.5f,      0.0f, 0.4f, 0.0f, // Top back

    // Top face
    -0.5f, 0.5f, 0.5f,      0.0f, 0.2f, 0.0f, // Front left
    0.5f, 0.5f, 0.5f,       0.0f, 0.2f, 0.0f, // Front right
    0.5f, 0.5f, -0.5f,      0.0f, 0.2f, 0.0f, // Back right
    -0.5f, 0.5f, -0.5f,     0.0f, 0.2f, 0.0f, // Back left

    // Bottom face
    -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, // Front left
    0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 0.0f, // Front right
    0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f, // Back right
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f  // Back left
};

GLuint indices[] = {
    0, 1, 2,    // Front face
    2, 3, 0,    // Front face
    4, 5, 6,    // Back face
    6, 7, 4,    // Back face
    8, 9, 10,   // Left face
    10, 11, 8,  // Left face
    12, 13, 14, // Right face
    14, 15, 12, // Right face
    16, 17, 18, // Top face
    18, 19, 16, // Top face
    20, 21, 22, // Bottom face
    22, 23, 20  // Bottom face
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            mousePressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        else if (action == GLFW_RELEASE)
        {
            mousePressed = false;
        }
    }
}

void compileShader(GLuint shader, const GLchar* source)
{
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0)
        zoomLevel += zoomSpeed;
    else if (yoffset < 0)
        zoomLevel -= zoomSpeed;

    // Limit the zoom level to a minimum value
    if (zoomLevel < 0.1f)
        zoomLevel = 0.1f;
}


void setupCallbacks(GLFWwindow* window)
{
    glfwSetScrollCallback(window, scrollCallback);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cube", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    setupCallbacks(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetKeyCallback(window, keyCallback);

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertexShader, vertexShaderSource);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragmentShader, fragmentShaderSource);

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (mousePressed)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            double deltaX = mouseX - lastMouseX;
            double deltaY = mouseY - lastMouseY;

            lastMouseX = mouseX;
            lastMouseY = mouseY;

            // Calculate rotation angles based on mouse movement
            GLfloat rotationAngleX = static_cast<GLfloat>(deltaY) * 0.005f;
            GLfloat rotationAngleY = static_cast<GLfloat>(deltaX) * 0.005f;

            // Apply rotations to the model matrix
            model = glm::rotate(glm::rotate(model, rotationAngleX, glm::vec3(1.0f, 0.0f, 0.0f)), rotationAngleY, glm::vec3(0.0f, 1.0f, 0.0f));
        }


        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(shaderProgram, "zoomLevel"), zoomLevel);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}



