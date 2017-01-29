#include "GLSLProgram.h"

#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram()
	: _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{
}

GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath)
{
	//Get a program object.
	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (!_vertexShaderID) {
		ErrorManager::error("Failed to create vertex shader!", ErrorLevel::FATAL, true);
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (!_fragmentShaderID) {
		ErrorManager::error("Failed to create fragment shader!", ErrorLevel::FATAL, true);
	}

	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilepath, _fragmentShaderID);
}

void GLSLProgram::compileShader(const std::string& filePath, GLuint id)
{
	std::ifstream file(filePath);
	if (file.fail()) {
		ErrorManager::error("Failed to open " + filePath + "!", ErrorLevel::FATAL, true);
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(file, line)) {
		fileContents += line + "\n";
	}

	file.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);
	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		//Provide the infolog in whatever manner you deem best.
		//Exit with failure.
		glDeleteShader(id); //Don't leak the shader.

		ErrorManager::error("Shader failed to compile!" + std::string(errorLog.begin(), errorLog.end()), ErrorLevel::FATAL, true);
	}
}

void GLSLProgram::linkShaders()
{
	//Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(_programID);
		//Don't leak shaders either.
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		//Use the infoLog as you see fit.
		ErrorManager::error("Shader failed to compile!" + std::string(errorLog.begin(), errorLog.end()), ErrorLevel::FATAL, true);
	}

	//Always detach shaders after a successful link.
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	//Delete shaders to free up resources
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::addAttribute(const std::string& attributeName)
{
	if (!_programID) {
		ErrorManager::error("GLSL Program was not created before adding attributes!", ErrorLevel::HIGH, false);
	}
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string& variableName)
{
	if (!_programID) {
		ErrorManager::error("GLSL Program was not created before getting uniform variable!", ErrorLevel::HIGH, false);
	}
	GLint location = glGetUniformLocation(_programID, variableName.c_str());
	if (location == -1) {
		ErrorManager::error("Variable name provided, " + variableName + ", was invalid!", ErrorLevel::MEDIUM, false);
	}
	else if (location == GL_INVALID_VALUE) {
		ErrorManager::error("Program ID provided was not generated by OpenGL!", ErrorLevel::HIGH, false);
	}
	else if (location == GL_INVALID_OPERATION) {
		ErrorManager::error("The GLSL Program associated with the provided Program ID is not a program object, or it was not successfully linked!", ErrorLevel::HIGH, false);
	}
	else {
		return location;
	}
	return -1;
}

void GLSLProgram::use()
{
	glUseProgram(_programID);
	for (size_t i = 0; i < _numAttributes; ++i) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse()
{
	glUseProgram(0);
	for (size_t i = 0; i < _numAttributes; ++i) {
		glDisableVertexAttribArray(i);
	}
}