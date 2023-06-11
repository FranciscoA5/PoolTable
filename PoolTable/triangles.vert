#version 330 core 
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor;
layout(location = 2) in vec3 normal; // Add input variable for normals
layout(location = 3) in vec2 uv; // Add input variable for UVs

out vec3 Normal; // Pass normals to the fragment shader
out vec2 TexCoord; // Pass UVs to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 color;
void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   color = aColor;
   Normal = mat3(transpose(inverse(model))) * normal;
   TexCoord = uv;
};