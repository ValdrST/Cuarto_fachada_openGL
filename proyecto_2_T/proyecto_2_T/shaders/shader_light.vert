#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

void main() 
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	TexCoord = tex;
	//para tomar las Transformaciones geométricas y la transpuesta es para que las escalas se inviertan en caso de escalas no uniformes en las normales
	Normal = mat3(transpose(inverse(model))) * norm;
	
	FragPos = (model * vec4(pos, 1.0)).xyz; //para obtener un vec3 sólo posición con respecto al origen en orden xyz: Swizzlin
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
}