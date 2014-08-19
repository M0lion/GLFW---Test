#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "VertexGenerator.h"
#include "ShaderUtils.h"

int windowWidth = 640;
int windowHeight = 480;

int time = 0;

bool aaEnabled = false;
bool spin = false;
bool clock = true;

int error;

static void keyHandler(GLFWwindow* sender, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
		case(GLFW_KEY_A):
			if (aaEnabled)
				glDisable(GL_LINE_SMOOTH);
			else
				glEnable(GL_LINE_SMOOTH);

			aaEnabled = !aaEnabled;
			break;

		case(GLFW_KEY_1) :
			spin = !spin;
			break;

		case(GLFW_KEY_2) :
			clock = !clock;
			break;
		default:
			break;
	}
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, keyHandler);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	//start glew
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertexShader = createShaderFromFile("basicShader.vsh", GL_VERTEX_SHADER);
	GLuint fragmentShader = createShaderFromFile("basicShader.fsh", GL_FRAGMENT_SHADER);

	glClearColor(0.005, 0, 0.12, 1);

	GLuint shaderProgram = glCreateProgram();

	if (shaderProgram == 0)
	{
		std::cout << "Could not create shader program!\n";
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "fragData");

	glLinkProgram(shaderProgram);

	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		std::cout << "Program link failed!" << std::endl;

		GLint infoLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);

		GLchar *info = new GLchar[infoLength + 1];
		glGetProgramInfoLog(shaderProgram, infoLength + 1, NULL, info);
		std::cout << info << std::endl;
		delete info;
	}

	glUseProgram(shaderProgram);

	//*random*data
	GLfloat *vertexData = 0;
	GLushort *indexData = 0;

	int vertices;
	int indices;
	vGen::generateCyllinder(vertexData, indexData, 5, 1, 1, &vertices, &indices);

	for (int i = 0; i < vertices; i++)
	{
		std::cout << vertexData[i * 4] << ", ";
		std::cout << vertexData[(i * 4) + 1] << ", ";
		std::cout << vertexData[(i * 4) + 2] << ", ";
		std::cout << vertexData[(i * 4) + 3] << "\n";
	}

	std::cout << std::endl;

	for (int i = 0; i < indices; i+=3)
	{
		std::cout << indexData[i] << ", ";
		std::cout << indexData[(i)+1] << ", ";
		std::cout << indexData[(i)+2] << ", ";
		std::cout << indexData[(i)+3] << "\n";
	}

	//create VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices*4, vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) *indices, indexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//create VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//set shader vars
	GLint positionHandle = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(positionHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(positionHandle, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

	//unbind stuff
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(NULL);

	GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

	GLint colorHandle = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(colorHandle, 0, 1, 0, 1);

	GLint functionHandle = glGetUniformLocation(shaderProgram, "time");
	glUniform1i(functionHandle, time);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);

	glm::mat4 projection;
	glm::mat4 view = glm::mat4();
	glm::mat4 model = glm::mat4();
	model = glm::scale(model, glm::vec3(5,5,5));

	view = glm::translate(view, glm::vec3(0,0,-10));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (error != GL_NO_ERROR)
			std::cout << "Error3: " << error << "\n";
		GLint viewPortWidth, viewPortHeight;
		glfwGetFramebufferSize(window, &viewPortWidth, &viewPortHeight);

		/* Render here */
		glViewport(0, 0, viewPortWidth, viewPortHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float aspect = (float)viewPortWidth / viewPortHeight;
		//projection = glm::ortho(-aspect, aspect, -1.0f, +1.0f, 0.0f, 100.0f);
		projection = glm::perspectiveFov(90.0f, aspect, 1.0f, 0.01f, 100.0f);

		const float angle = 1;

		if (spin)
			model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		if(clock)
			time++;

		glUniform1i(functionHandle, time);
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}