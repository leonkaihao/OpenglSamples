#pragma once

#include <GL\glew.h>

class MyTexture
{
public:
	MyTexture();
	~MyTexture();
	int loadImageData(const char* fileName);
	inline GLuint getID() { return m_Texture; }
	inline int getWidth() { return m_iWidth; }
	inline int getHeight() { return m_iHeight; }

private:
	static void _Init(void);
	static bool m_bInitialized;
	static int m_iCount;
	int m_iWidth, m_iHeight;
	GLuint m_Texture;
};

