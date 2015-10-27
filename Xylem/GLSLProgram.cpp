#include "GLSLProgram.h"

#include "ErrorManager.h"

#include <vector>
#include <fstream>

namespace Xylem
{
    GLSLProgram::GLSLProgram()
        : _numberOfAttributes(0),
        _programID(0),
        _vertexShaderID(0),
        _fragmentShaderID(0)
    {
    }

    GLSLProgram::~GLSLProgram()
    {
    }

    /// Compile the GLSL Shaders.
    void GLSLProgram::compileShaders(const std::string & vertexShaderFilePath, const std::string & fragmentShaderFilePath)
    {
        _programID = glCreateProgram();

        _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if (_vertexShaderID == 0) {
            ErrorManager::fatalError("Vertex shader could not be created!");
        }

        _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if (_fragmentShaderID == 0) {
            ErrorManager::fatalError("Fragment shader could not be created!");
        }

        compileShader(_vertexShaderID, vertexShaderFilePath);
        compileShader(_fragmentShaderID, fragmentShaderFilePath);
    }

    /// Link the shaders to the shader program.
    void GLSLProgram::linkShaders()
    {
        glAttachShader(_programID, _vertexShaderID);
        glAttachShader(_programID, _fragmentShaderID);

        glLinkProgram(_programID);

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

            //print the error log and quit
            std::printf("%s\n", &(errorLog[0]));
            ErrorManager::fatalError("Shaders failed to link!");
        }

        //Always detach shaders after a successful link.
        glDetachShader(_programID, _vertexShaderID);
        glDetachShader(_programID, _fragmentShaderID);
        glDeleteShader(_vertexShaderID);
        glDeleteShader(_fragmentShaderID);
    }

    /// Adds an attribute to the shader program.
    void GLSLProgram::addAttribute(const std::string attributeName)
    {
        glBindAttribLocation(_programID, _numberOfAttributes++, attributeName.c_str());
    }

    /// Gets the location of a uniform.
    GLuint GLSLProgram::getUniformLocation(const std::string & uniformName)
    {
        GLuint location = glGetUniformLocation(_programID, uniformName.c_str());
        if (location == GL_INVALID_INDEX) {
            ErrorManager::fatalError("Uniform " + uniformName + " not found in shader!");
        }
        return location;
    }

    /// Uses the shader program, enabling the associated vertex attributes.
    void GLSLProgram::use()
    {
        glUseProgram(_programID);
        for (size_t i = 0; i < _numberOfAttributes; ++i) {
            glEnableVertexAttribArray(i);
        }
    }

    /// Unues the shader program, disabling the associated vertex attributes.
    void GLSLProgram::unuse()
    {
        glUseProgram(0);
        for (size_t i = 0; i < _numberOfAttributes; ++i) {
            glDisableVertexAttribArray(i);
        }
    }

    /// Compiles an individual shader.
    void GLSLProgram::compileShader(GLuint shaderID, const std::string & shaderFilePath)
    {
        std::ifstream file(shaderFilePath);
        if (file.fail()) {
            perror(shaderFilePath.c_str());
            ErrorManager::fatalError("Failed to open " + shaderFilePath + "!");
        }

        std::string fileContents = "";
        std::string line;
        while (std::getline(file, line)) {
            fileContents += line + "\n"; // Must add new line as new line character is ignored by getline().
        }

        file.close();

        const char* contentsPtr = fileContents.c_str();
        glShaderSource(shaderID, 1, &contentsPtr, nullptr);

        glCompileShader(shaderID);

        GLint success = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character.
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

            glDeleteShader(shaderID);

            std::printf("%s\n", &errorLog[0]);
            ErrorManager::fatalError("Shader " + shaderFilePath + " failed to compile.");
        }
    }
}