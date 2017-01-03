#include "MyTexture.h"
#include <iostream>
#include <SOIL.h>
using namespace std;


bool MyTexture::m_bInitialized = false;
int MyTexture::m_iCount = 0;

MyTexture::MyTexture() :m_iWidth(0), m_iHeight(0), m_Texture(0)
{
	_Init();
}


MyTexture::~MyTexture()
{
}

int MyTexture::loadImageData(const char* fileName) {

	int width, height;
	unsigned char* image = 0;
	image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0) {
		cout << "Fail to load image " << fileName << endl;
		return -1;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	m_Texture = texture;
	m_iWidth = width;
	m_iHeight = height;
	return 0;
}

void MyTexture::_Init(void) {
	if (m_bInitialized) {
		return;
	}
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_bInitialized = true;
}