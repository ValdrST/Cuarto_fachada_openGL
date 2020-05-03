#include "Texture.h"
#include "CommonValues.h"


Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = 0;
}
Texture::Texture(const char *FileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = FileLoc;
}

bool Texture::LoadTexture()
{
	//para cambiar el origen a la esquina inferior izquierda como necesitamos
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth,STBI_default); //el tipo unsigned char es para un array de bytes de la imagen, obtener datos de la imagen 
	if (!texData)
	{
		printf("No se encontr� el archivo: %s", fileLocation);
	}
	GLenum format;
	if (bitDepth == 1)
		format = GL_RED;
	else if (bitDepth == 3)
		format = GL_RGB;
	else if (bitDepth == 4)
		format = GL_RGBA;
	glGenTextures(1, &textureID); //parecido al VAO: crear una textura y asignarle un �ndice
	glBindTexture(GL_TEXTURE_2D, textureID);//se indica que la textura es de tipo 2D, para superficies planas es suficiente esta textura
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// eje S paralelo a X, repetir sobre el eje
/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);// eje S paralelo a X, repetir sobre el eje pero rotando con forme a un centro
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);// eje S paralelo a X, envolver toda la superficie
*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// eje T paralelo a Y, repetir sobre el eje
	/*GL_TEXTURE_MIN_FILTER: Para m�s cerca o textura se escala a menor tama�o. GL_TEXTURE_MAG_FILTER: Para m�s lejos o textura se escala a mayor tama�o. 
	GL_LINEAR  aplica sampling y blending de texels m�s cercanos. GL_NEAREST aplica sample de texel m�s cercano
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//if(RGBA) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	/*}else{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData)
	glGenerateMipmap(GL_TEXTURE_2D); */
	glBindTexture(GL_TEXTURE_2D, 0);//para hacer un unbind de la textura
	stbi_image_free(texData); //para liberar la informaci�n de la imagen
	///
	///
	// shadow
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void Texture::ClearTexture()
{

	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}
void Texture::UseTexture()
{	//UnitTexture
	glActiveTexture(GL_TEXTURE0); //para crear un sampler que es lo que necesitan los shaders para poder acceder a la textura: 16 a 32 texturas pueden ser declaradas
	//si hay mas de 1 unittexture se tiene que crear una unifromvariable que haga cambio entre la unit texture a utilizar
	glBindTexture(GL_TEXTURE_2D, textureID);

}

Texture::~Texture()
{
	ClearTexture();
}
