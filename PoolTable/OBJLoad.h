#ifndef OBJLOAD_H
#define OBJLOAD_H
#include <GL/glew.h> // Include the OpenGL headers here
#include <string>
#include <vector>    
#include <glm/glm.hpp>

namespace OBJLoad {
    class Ball {

    private:
        GLuint VAO; // Member variable to store the VAO
       

    public:

        // Load the vertex data for the cube
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        //void Read(const std::string& obj_model_filepath);
        void Send();
        void Draw(glm::vec3 position, glm::vec3 orientation);
        bool loadOBJ(
            const char* path,
            std::vector < glm::vec3 >& out_vertices,
            std::vector < glm::vec2 >& out_uvs,
            std::vector < glm::vec3 >& out_normals
        );
        void ReadMTL( char* mtl_model_filepath);
        void LoadTexture(const char* textureFileName, GLuint textureName);
    };

} // namespace MyLibrary

#endif // MYLIBRARY_HPP
