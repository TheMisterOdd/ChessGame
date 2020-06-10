#pragma once

#include "Window.h"

char* ReadFile(const char* path)
{

	char* buffer = NULL;
	long length;
	FILE* f = NULL;
	fopen_s(&f, path, "rb"); //was "rb"
	assert(f != NULL);

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char*)malloc((length + 1) * sizeof(char));
		if (buffer)
		{
			fread(buffer, sizeof(char), length, f);
		}

		fclose(f);
	}
	else
	{
		char* err = (char*)malloc(sizeof(char) * 128);
		strerror_s(err, 128, errno);

		printf("Error: %s: %s\n", path, err);
		free(err);

		return NULL;
	}
	buffer[length] = '\0';

	return buffer;
}

unsigned int CreateShader(const char* source, GLenum type)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int compile;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile);

	if (compile == GL_FALSE)
	{
		int lenght;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
		char* buffer = (char*)calloc(lenght, sizeof(char));

		glGetShaderInfoLog(shader, lenght, &lenght, buffer);
		fprintf(
			stderr,
			"\n[Error]: shader '%s': info:\n%s\n",
			type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER", buffer
		);
	}

	return shader;
}

unsigned int NewShader(const char* vertPath, const char* fragPath)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CreateShader(ReadFile(vertPath), GL_VERTEX_SHADER);
	unsigned int fs = CreateShader(ReadFile(fragPath), GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}