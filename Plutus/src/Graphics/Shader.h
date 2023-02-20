#ifndef _SHADER_H
#define _SHADER_H

#include <iostream>
#include "GLheaders.h"
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

#include <Math/Vectors.h>

namespace Plutus
{
	/*
		Help create a opengl shader program
	*/
	class Shader
	{
	private:
		//Shader Program Id
		uint32_t mProgId = -1;
		//A map of uniform locations
		std::unordered_map<std::string, GLint> mUniforms;

	public:
		Shader() = default;
		~Shader();
		/*Create a shader program
			@param verShader path to a vertex shader file or string with the data
			@param fragShader path to a vertex fragment shader file or string with the data
			@param optional geoShader path to a vertex geometry shader file or string with the data
		*/
		bool init(const std::string& verShader = "", const std::string& fragShader = "", const std::string geoShader = "");
		// Enable this shader program
		bool enable();
		// Disable this shader program
		void disable();
		// Destroy this shader program
		void destroy();

		uint32_t getUniform(std::string uName);
		// Return the shader program ID
		uint32_t getProgId() { return mProgId; }
		//Upload Uniform 1 boolean 
		void setUniform1b(std::string name, GLboolean value);
		//Upload Uniform 1 integer
		void setUniform1i(std::string name, GLuint value);
		//Uniform 1 variable Array
		void setUniform1iv(std::string name, GLint* value, GLuint size);
		//Upload Uniform 1 float
		void setUniform1f(std::string name, float value);
		//Upload Uniform 2 float
		void setUniform2f(std::string name, const Vec2f& value);
		//Upload Uniform 3 float
		void setUniform3f(std::string name, const Vec3f& value);
		//Upload Uniform 4 float
		void setUniform4f(std::string name, const Vec4f& value);
		//Upload Uniform array of float
		void setUniform1fv(std::string name, int count, float* buffer);
		//Upload Uniform array of int
		void setUniform1iv(std::string name, int count, int* buffer);
		//Upload Uniform matrix 4x4 of float
		void setUniformMat4(std::string name, const glm::mat4& value);

	private:
		bool compileShader(uint32_t id, std::string shader);
		bool getCompileError(uint32_t id, std::string shader);
		void cleanUp(GLuint verId, GLuint fragId);
	};

} // namespace Plutus

#endif