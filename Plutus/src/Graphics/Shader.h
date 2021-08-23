#ifndef _SHADER_H
#define _SHADER_H

#include <iostream>
#include "GLheaders.h"
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace Plutus
{
	/*
		Help create a opengl shader program
	*/
	class Shader
	{
	private:
		bool compileError = false;
		//Number of attributes for shader
		int mNumAttributes = 0;
		//Shader Program Id
		uint32_t mProgId = -1;
		//A map of uniform locations
		std::unordered_map<std::string, GLint> mUniforms;

	public:
		Shader() = default;
		~Shader();
		/*Create a shader program
			@param vsPath path to a vertex shader file or string with the data
			@param fsPath path to a vertex fragment shaer file or string with the data
		*/
		bool CreateProgWithShader(std::string vsPath, std::string fsPath);
		// Enable this shader program
		void enable();
		// Disable this shader program
		void disable();
		// Destroy this shader program
		void dispose();

		uint32_t getUniform(std::string uName);
		// Return the shader program ID
		uint32_t getProg() { return mProgId; }
		// Acivate attributes by name
		void setAtribute(const std::string& attributeName);
		//Upload Uniform 1 integer
		void setUniform1i(std::string name, GLuint value);
		//Uniform 1 variable Array
		void setUniform1iv(std::string name, GLint* value, GLuint size);
		//Upload Uniform 1 float
		void setUniform1f(std::string name, float value);
		//Upload Uniform 2 float
		void setUniform2f(std::string name, glm::vec2 value);
		//Upload Uniform 3 float
		void setUniform3f(std::string name, glm::vec3 value);
		//Upload Uniform 4 float
		void setUniform4f(std::string name, const glm::vec4& value);
		//Upload Uniform matrix 4x4 of float
		void setUniformMat4(std::string name, const glm::mat4& value);

	private:
		void compileShader(uint32_t id, std::string shader);
		bool getCompileError(uint32_t id, std::string shader);
		void cleanUp(GLuint verId, GLuint fragId);
	};

} // namespace Plutus

#endif