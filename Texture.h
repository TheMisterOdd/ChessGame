#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

typedef struct 
{
	int width, height, channels;
	unsigned int m_TextureID;
} Texture;

void NewTexture(Texture* self, const char* path) 
{
#ifdef _DEBUG
	printf("Loading texture: '%s'...", path);
	double initialTime = glfwGetTime();
#endif // _DEBUG

	stbi_set_flip_vertically_on_load(1);
	unsigned char* img = stbi_load(path, &self->width, &self->height, &self->channels, STBI_rgb_alpha);
	stbi_set_flip_vertically_on_load(0);

	if (img == NULL)
	{
		fprintf(stderr, "[Error]: Cannot open given file '%s'.\n", path);
		return;
	}

	glGenTextures(1, &self->m_TextureID);
	glBindTexture(GL_TEXTURE_2D, self->m_TextureID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, self->width, self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img);

#ifdef _DEBUG
	double finalTime = glfwGetTime();
	printf(" Elapsed time for: 'NewTexture()': %f\n", finalTime - initialTime);
#endif // _DEBUG

}

#define TextureBind(self, sampler)\
	glActiveTexture(GL_TEXTURE0 + sampler);\
	glBindTexture(GL_TEXTURE_2D, self.m_TextureID)

#define TextureUnbind() glBindTexture(GL_TEXTURE_2D, 0)

#define TextureDelete(self) glDeleteTextures(1, &self.m_TextureID)

