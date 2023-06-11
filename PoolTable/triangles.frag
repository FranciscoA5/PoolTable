#version 330 core
out vec4 FragColor; 
in vec3 color; 
in vec3 Normal; // Receive normals from the vertex shader
in vec2 TexCoord; // Receive UVs from the vertex shader

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D texture1; // If you're using textures, add this uniform
void main()
{  
    // Perform lighting calculations here using the normals and UVs
   
   FragColor = vec4(1f) * texture(texture1, TexCoord);
   // Example: Apply texture color using UV coordinates
   vec3 texColor = texture(texture1, TexCoord).rgb;
   //FragColor = vec4(color, 1.0f);
};