#include "shader.h"

void Shader::loadUniformValue(GLuint uniformLocation, int value){
    glUniform1i(uniformLocation, value);
}

void Shader::loadUniformValue(GLuint uniformLocation, float value){
    glUniform1f(uniformLocation, value);
}

void Shader::loadUniformValue(GLuint uniformLocation, glm::vec2 value){
    glUniform2fv(uniformLocation, 1, &value.x);
}

void Shader::loadUniformValue(GLuint uniformLocation, glm::vec3 value){
    glUniform3fv(uniformLocation, 1, &value.x);
}

void Shader::loadUniformValue(GLuint uniformLocation, glm::vec4 value){
    glUniform4fv(uniformLocation, 1, &value.x);
}

void Shader::loadUniformValue(GLuint uniformLocation, glm::mat2 value){
    glUniformMatrix2fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void Shader::loadUniformValue(GLuint uniformLocation, glm::mat3 value){
    glUniformMatrix3fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void Shader::loadUniformValue(GLuint uniformLocation, glm::mat4 value){
    glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void Shader::loadUniformValue(GLuint uniformLocation, float* value, int count){
    switch(count){
        case 1:
            glUniform1f(uniformLocation, *value);
            break;
        case 2:
            glUniform2fv(uniformLocation, 1, value);
            break;
        case 3:
            glUniform3fv(uniformLocation, 1, value);
            break;
        case 4:
            glUniform4fv(uniformLocation, 1, value);
            break;
        default:
            std::cerr << "Cant load uniform with " << count << " dimensions." << std::endl;
    }
}

void Shader::loadLightUniform(std::string property, int index, glm::vec3 value) {
		std::ostringstream ss;
		ss << "lights[" << index << "]." << property;
		std::string uniformName = ss.str();

		GLuint uniform_location = glGetUniformLocation(ID, uniformName.c_str());
		loadUniformValue(uniform_location, value);
	}
	void Shader::loadLightUniform(std::string property, int index, glm::vec4 value) {
		std::ostringstream ss;
		ss << "lights[" << index << "]." << property;
		std::string uniformName = ss.str();

		GLuint uniform_location = glGetUniformLocation(ID, uniformName.c_str());
		loadUniformValue(uniform_location, value);
	}
	void Shader::loadLightUniform(std::string property, int index, float value) {
		std::ostringstream ss;
		ss << "lights[" << index << "]." << property;
		std::string uniformName = ss.str();

		GLuint uniform_location = glGetUniformLocation(ID, uniformName.c_str());
		loadUniformValue(uniform_location, value);
	}