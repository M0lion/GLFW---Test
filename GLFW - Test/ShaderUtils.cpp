#include "ShaderUtils.h"

GLuint createShaderFromFile(GLchar *path, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);

	std::ifstream inStream;
	inStream.open(path);
	if (!inStream.is_open())
	{
		inStream.close();
		std::cout << "file not found: '" << path << "'!" << std::endl;
		return 0;
	}

	std::string source((std::istreambuf_iterator<GLchar>(inStream)), std::istreambuf_iterator<GLchar>());
	inStream.close();

	const GLchar *shaderSource = source.c_str();

	glShaderSource(shaderID, 1, &shaderSource, NULL);

	glCompileShader(shaderID);

	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		std::cout << "Shader failed to compile: '" << path << "'!" << std::endl;
		
		GLint infoLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength);

		GLchar *info = new GLchar[infoLength + 1];
		glGetShaderInfoLog(shaderID, infoLength + 1, NULL, info);
		std::cout << info << std::endl;
		delete info;
	}

	return shaderID;
}