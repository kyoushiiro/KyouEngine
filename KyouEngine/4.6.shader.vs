#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0f);
   FragPos = vec3(model * vec4(aPos, 1.0));
   // Dont forget to multiply by normal matrix if using non-uniform scales
   Normal = aNormal;
   TexCoords = aTexCoords;
}
