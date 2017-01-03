#pragma once

#include <GL/glew.h> // Include glew to get all the required OpenGL headers

class MyShader
{
public:
	// The program ID
	GLuint Program;
	// Constructor reads and builds the shader
	MyShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~MyShader();
	// Use the program
	void Use();
};

