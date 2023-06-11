#define _CRT_SECURE_NO_WARNINGS
#include "OBJLoad.h"
#include <vector>              // For std::vector
#include <glm/glm.hpp>// For glm::vec2, glm::vec3
#include <iostream>

#include"stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <string> // Include the <string> header for std::string




using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>
#include"LoadShaders.h"



namespace OBJLoad {

    bool Ball::loadOBJ(const char* path, std::vector < glm::vec3 >& out_vertices, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec3 >& out_normals) {


        std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
        std::vector< glm::vec3 > temp_vertices;
        std::vector< glm::vec2 > temp_uvs;
        std::vector< glm::vec3 > temp_normals;

        //FILE* file = fopen(path, "r");
        FILE* file;
        errno_t err;
        err = fopen_s(&file, path, "r");
        if (file == NULL) {
            printf("Impossible to open the file !\n");
            printf(path);
            return false;
        }

        while (1) {

            char lineHeader[128];
            // read the first word of the line
            int res = fscanf_s(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop

            

           if (strcmp(lineHeader, "mtllib") == 0) {
              
               char mtlFilename[256];
              fscanf_s(file, "%s", mtlFilename, sizeof(mtlFilename));
              printf("%s",mtlFilename);

             // std::string mtlFilenameStr(mtlFilename);
             
              ReadMTL(mtlFilename);


             
           }


            
            else if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 vertex;
                fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader, "vt") == 0) {
                glm::vec2 uv;
                fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
                temp_uvs.push_back(uv);
            }
            else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 normal;
                fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            }
            else if (strcmp(lineHeader, "f") == 0) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9) {
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");

                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }

        // For each vertex of each triangle
        for (unsigned int i = 0; i < vertexIndices.size(); i++) {

            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = temp_vertices[vertexIndex - 1];
            out_vertices.push_back(vertex);

            //pega nas normais da superficie
            unsigned int normalIndex = normalIndices[i];
            glm::vec3 normal = temp_normals[normalIndex - 1];
            out_normals.push_back(normal);

            //pegar nos uvs(texture coordinates)
            unsigned int uvIndex = uvIndices[i];
            glm::vec2 uv = temp_uvs[uvIndex - 1];
            out_uvs.push_back(uv);
        }


    }

    void Ball::Send() {



        ShaderInfo  shaders[] = {
        { GL_VERTEX_SHADER,   "triangles.vert" },
        { GL_FRAGMENT_SHADER, "triangles.frag" },
        { GL_NONE, NULL }
        };

        GLuint shaderProgram = LoadShaders(shaders);
        if (!shaderProgram) exit(EXIT_FAILURE);
        glUseProgram(shaderProgram);

        //VBO é um array de referencias ,mas como so temos um objeto so precisamos de usar um 
        GLuint  VBO;

        // Generate and bind the VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

       

        

        // Apply scaling to reduce the size of the object
        float scale = 0.10f; // Adjust the scale factor as needed
        for (size_t i = 0; i < vertices.size(); i++) {
            vertices[i] *= scale;
        }

        // Store the vertex data in the VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        // Configure the vertex attribute pointer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Store the UV data in a new VBO
        GLuint uvsVBO;
        glGenBuffers(1, &uvsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

        // Configure the vertex attribute pointers for UVs
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(3);

        // Store the normal data in a new VBO
        GLuint normalsVBO;
        glGenBuffers(1, &normalsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

        // Configure the vertex attribute pointers for normals
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);


        GLint textura = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "texture1");
        glProgramUniform1i(shaderProgram, textura, 0);



    }

    void Ball::Draw(glm::vec3 position, glm::vec3 orientation) {
        // Implemente a renderização do modelo na posição e orientação fornecidas aqui
        // Bind the VAO
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    void Ball::ReadMTL( char*  mtl_model_filepath) {

        char lineHeader[128];

        FILE* mtlFile;
        errno_t err;
        err = fopen_s(&mtlFile, mtl_model_filepath, "r");
        if (mtlFile == NULL) {
            printf("Impossible to open the file !\n");
            return;
        }

        while (lineHeader[0] != EOF) {
             
            
         
            int res = fscanf_s(mtlFile, "%s", lineHeader, sizeof(lineHeader));
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop

            
            if (strcmp(lineHeader, "Ka") == 0) {
                glm::vec3 ambientColor;
                fscanf_s(mtlFile, "%f %f %f", &ambientColor.r, &ambientColor.g, &ambientColor.b);

                // ambientColor now contains the ambient color values (RGB)
                // You can process it further or store it as needed
            }
            else if (strcmp(lineHeader, "Kd") == 0) {
                glm::vec3 diffuseColor;
                fscanf_s(mtlFile, "%f %f %f", &diffuseColor.r, &diffuseColor.g, &diffuseColor.b);

                // diffuseColor now contains the diffuse color values (RGB)
                // You can process it further or store it as needed
            }
            else if (strcmp(lineHeader, "Ks") == 0) {
                glm::vec3 specularColor;
                fscanf_s(mtlFile, "%f %f %f", &specularColor.r, &specularColor.g, &specularColor.b);

                // specularColor now contains the specular color values (RGB)
                // You can process it further or store it as needed
            }
            else if (strcmp(lineHeader, "Ns") == 0) {
                float shininess;
                fscanf_s(mtlFile, "%f", &shininess);

                // shininess now contains the specular exponent (shininess value)
                // You can process it further or store it as needed
            }
            else if (strcmp(lineHeader, "map_Kd") == 0) {
                char textureFilename[128];
                fscanf_s(mtlFile, "%s", textureFilename, sizeof(textureFilename));
                LoadTexture(textureFilename , 0);

                // textureFilename now contains the name of the texture image file
                // You can process it further or store it as needed
            }
        }

    } // namespace MyLibrary

    void Ball::LoadTexture(const char* textureFileName , GLuint textureName) {

      
     

        // Gera um nome de textura
        glGenTextures(1, &textureName);

        // Ativa a Unidade de Textura #0
        // A Unidade de Textura 0 já está ativa por defeito.
        // Só uma Unidade de Textura está ativa a cada momento.
        glActiveTexture(GL_TEXTURE0);

        // Vincula esse nome de textura ao target GL_TEXTURE_2D da Unidade de Textura ativa.
        glBindTexture(GL_TEXTURE_2D, textureName);

        // Define os parâmetros de filtragem (wrapping e ajuste de tamanho)
        // para a textura que está vinculada ao target GL_TEXTURE_2D da Unidade de Textura ativa.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Leitura/descompressão do ficheiro com imagem de textura
        int width, height, nChannels;
        // Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
        stbi_set_flip_vertically_on_load(true);
        // Leitura da imagem para memória do CPU
        unsigned char* imageData = stbi_load(textureFileName, &width, &height, &nChannels, 0);
        if (imageData) {
            // Carrega os dados da imagem para o Objeto de Textura vinculado ao target GL_TEXTURE_2D da Unidade de Textura ativa.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

            // Gera o Mipmap para essa textura
            glGenerateMipmap(GL_TEXTURE_2D);

            // Liberta a imagem da memória do CPU
            stbi_image_free(imageData);
        }
        else {
            std::cout << "Error loading texture!" << std::endl;
        }
    }
}