#include <iostream>  
#include <map>
// GLEW  
#define GLEW_STATIC  
#include <GL/glew.h>  
// GLFW  
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"

#include "MyShader.h"
#include "MyTexture.h"
#include "text_renderer.h"

// Function prototypes  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions  
const GLuint WIDTH = 1600, HEIGHT = 800;

//hk: initial vertex array
GLfloat vertices[] = {
	// Positions          // Colors           // Texture Coords
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Top Right
	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Bottom Right
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // Bottom Left
	-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // Top Left 
};
GLuint indices[] = {  // Note that we start from 0!
	//0, 1, 2   // First Triangle
	0, 1, 3,   // First Triangle
	3, 2, 1    // Second Triangle
};


//argument
int ilAperture = 0;
int ilRedius = 1024, ilCentreX = 1024, ilCentreY = 1024;
int ilRotateX = 0, ilRotateY = 0, ilRotateZ = 0;

int irAperture = 0;
int irRedius = 1024, irCentreX = 1024, irCentreY = 1024;
int irRotateX = 0, irRotateY = 0, irRotateZ = 0;

// The MAIN function, from here we start the application and run the game loop  
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW  
	glfwInit();
	// Set all the required options for GLFW  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions  
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	// Set the required callback functions  
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions  
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers  
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	MyTexture texLeft, texRight;
	texLeft.loadImageData("./sample/1.jpg");
	texRight.loadImageData("./sample/2.jpg");
	// Define the viewport dimensions  
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, WIDTH, HEIGHT);
	///////////////////////hk: vertex shader start
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	
	//initial vertex buffer objects
	glBindVertexArray(VAO);
	//----------
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// texure attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//----------
	glBindVertexArray(0);

	MyShader fisheyeShader("fisheye.vsh", "fisheye.fsh");

	TextRenderer textRender(width, height);
	textRender.Load("consola.ttf", 24);
	int mode = GL_FILL;
	glEnable(GL_BLEND);
	// Game loop  
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions  
		//glfwPollEvents();
		glfwPollEvents();
		// Render  
		// Clear the colorbuffer  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		fisheyeShader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texLeft.getID());
		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "leftTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texRight.getID());
		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "rightTexture"), 1);

		glUniform1f(glGetUniformLocation(fisheyeShader.Program, "winWidth"), (GLfloat)WIDTH);
		glUniform1f(glGetUniformLocation(fisheyeShader.Program, "winHeight"), (GLfloat)HEIGHT);
		
		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "lrX"), (GLint)ilRotateX);
		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "lrY"), (GLint)ilRotateY);
		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "lrZ"), (GLint)ilRotateZ);

		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "rrX"), (GLint)irRotateX);
		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "rrY"), (GLint)irRotateY);
		glUniform1i(glGetUniformLocation(fisheyeShader.Program, "rrZ"), (GLint)irRotateZ);

		glBindVertexArray(VAO);
		////////////////////////////////////////////////////draw
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		////////////////////////////////////////////////////
		glBindVertexArray(0);

		char caliInfo[64];
		sprintf_s(caliInfo, "left: rx:%d ry:%d rz:%d      right: rx:%d ry:%d rz:%d",
			ilRotateX, ilRotateY, ilRotateZ, irRotateX, irRotateY, irRotateZ);

		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::vec3 color(1.0, 0, 0);
		textRender.RenderText(caliInfo, 5.0f, 5.0f, 1.0f, color);

		// Swap the screen buffers  
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.  
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
	std::cout << key << std::endl;
	if (action != GLFW_PRESS)
		return;
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	} else {
		int isLeft = (mode & GLFW_MOD_CONTROL);
		switch (key)
		{
		case GLFW_KEY_W: //look up
			if (isLeft) {
				if (ilRotateX < 90)
					ilRotateX += 1;
			} else {
				if (irRotateX < 90)
					irRotateX += 1;
			}
			break;
		case GLFW_KEY_S: //look down
			if (isLeft) {
				if (ilRotateX > -90)
					ilRotateX -= 1;
			}
			else {
				if (irRotateX > -90)
					irRotateX -= 1;
			}
			break;
		case GLFW_KEY_A: //look left
			if (isLeft) {
				if (ilRotateZ > -90)
					ilRotateZ -= 1;
			}
			else {
				if (irRotateZ > -90)
					irRotateZ -= 1;
			}
			break;
		case GLFW_KEY_D: //look right

			if (isLeft) {
				if (ilRotateZ < 90)
					ilRotateZ += 1;
			}
			else {
				if (irRotateZ < 90)
					irRotateZ += 1;
			}
			break;
		case GLFW_KEY_Q: //clockwise
			if (isLeft) {
				if (ilRotateY > -90)
					ilRotateY -= 1;
			}
			else {
				if (irRotateY > -90)
					irRotateY -= 1;
			}
			break;
		case GLFW_KEY_E: //anticlockwise
			if (isLeft) {
				if (ilRotateY < 90)
					ilRotateY += 1;
			}
			else {
				if (irRotateY < 90)
					irRotateY += 1;
			}
			break;
		default:
			break;
		}
	}
}