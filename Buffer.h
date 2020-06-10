#pragma once

#include <glad/glad.h>

/* Creates a 'GL_ARRAY_BUFFER' */
GLuint NewVBO(const void* data, size_t size)
{	
	GLuint m_RendererID;
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return m_RendererID;
}

#define VBO_Bind(Buffer) glBindBuffer(GL_ARRAY_BUFFER, Buffer)

#define VBO_Unbind() glBindBuffer(GL_ARRAY_BUFFER, 0)

#define VBO_Delete(Buffer) glDeleteBuffers(1, &Buffer)


/* Creates a 'GL_ELEMENT_ARRAY_BUFFER' */
GLuint NewIBO(const void* data, size_t count, size_t size)
{		
	GLuint m_RendererID;
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return m_RendererID;
}

#define IBO_Bind(Buffer) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer)

#define IBO_Unbind() glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)

#define IBO_Delete(Buffer) glDeleteBuffers(1, &Buffer)


GLuint NewVAO() 
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	return vao;
}

#define VAO_Bind(Array) glBindVertexArray(Array)

#define VAO_Unbind() glBindVertexArray(0)

#define VAO_Delete(Array) glDeleteVertexArrays(1, &Array)

size_t NewModel(unsigned int* vao, unsigned int* vbo, unsigned int* ibo)
{
	float vertices[] =
	{	// Positions:				Color:
		-1.0f, -1.0f, +0.0f,		+1.0f, +1.0f, +1.0f,		+0.0f, +0.0f,
		+1.0f, -1.0f, +0.0f,		+1.0f, +1.0f, +1.0f,		+1.0f, +0.0f,
		+1.0f, +1.0f, +0.0f,		+1.0f, +1.0f, +1.0f,		+1.0f, +1.0f,
		-1.0f, +1.0f, +0.0f,		+1.0f, +1.0f, +1.0f,		+0.0f, +1.0f
	};


	unsigned char indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	size_t count = 6;

	*vao = NewVAO();

	*vbo = NewVBO(vertices, sizeof(float) * 32);
	VBO_Bind(*vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	*ibo = NewIBO(indices, count, sizeof(unsigned char));
	IBO_Bind(*ibo);

	VAO_Unbind();
	VBO_Unbind();
	IBO_Unbind();

	return count;
}

void ModelBeginDraw(unsigned int vao) 
{
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}


void ModelEndDraw()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

/*
{
			glUseProgram(shader);

			glBindVertexArray(vao);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (float*)projection);
			glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)view);


			for (int i = 8; i > 0; --i)
			{
				for (int j = 8; j > 0; --j)
				{
					if ((i + j) % 2 == 0)
					{
						TextureBind(tile1, 0);
					}
					else
					{
						TextureBind(tile2, 0);
					}


					float x = (i - j);
					float y = (i + j) * 0.5f;

					glm_translate_make(object, (vec3) { x, y + 0.25f * sin(glfwGetTime()) - 5.0f , 0 });
					glUniformMatrix4fv(obj_loc, 1, GL_FALSE, (float*)object);

					glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, NULL);
					TextureUnbind();
				}

			}

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			glBindVertexArray(0);
			glUseProgram(0);
		}
*/