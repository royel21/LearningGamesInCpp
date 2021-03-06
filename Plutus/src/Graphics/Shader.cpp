#include "Shader.h"

#include <string>
#include <fstream>
#include <vector>
#include <cstdio>

#include <Utils/FileIO.h>
#include <Log/Logger.h>

#include "GLSL.h"

namespace Plutus
{
	Shader::~Shader()
	{
		destroy();
	}

	bool Shader::init(const std::string& verData, const std::string& fragData)
	{
		auto vsdata = verData.empty() ? GLSL::vertexShader : verData;
		auto fsdata = fragData.empty() ? GLSL::fragShader : fragData;

		mProgId = glCreateProgram();

		GLuint vertId = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);

		compileShader(vertId, vsdata);
		compileShader(fragId, fsdata);

		if (compileError)
		{
			cleanUp(vertId, fragId);
			return false;
		}


		int result = vertId && fragId;

		if (result)
		{
			//attach the shader to the program
			glAttachShader(mProgId, vertId);
			glAttachShader(mProgId, fragId);
			//link the programs
			glLinkProgram(mProgId);

			glGetProgramiv(mProgId, GL_LINK_STATUS, &result);
			if (result == GL_FALSE)
			{
				int InfogLenght;
				glGetProgramiv(mProgId, GL_INFO_LOG_LENGTH, &InfogLenght);
				std::vector<char> ProgramErrorMessage(InfogLenght + 1);
				glGetProgramInfoLog(mProgId, InfogLenght, NULL, &ProgramErrorMessage[0]);
				Logger::error("ProgError: %s\n", &ProgramErrorMessage[0]);
			}

			glDetachShader(mProgId, vertId);
			glDetachShader(mProgId, fragId);
		}

		glDeleteShader(vertId);
		glDeleteShader(fragId);
		return result;
	}

	void Shader::enable()
	{
		glUseProgram(mProgId);
		//enable all the attributes we added with addAttribute
		for (int i = 0; i < mNumAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void Shader::disable()
	{
		glUseProgram(0);
		//disable all the attributes we added with addAttribute
		for (int i = 0; i < mNumAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void Shader::destroy()
	{
		if (mProgId)
			glDeleteProgram(mProgId);
	}

	uint32_t Shader::getUniform(std::string uName)
	{
		if (mUniforms.find(uName) != mUniforms.end())
			return mUniforms[uName];

		return mUniforms[uName] = glGetUniformLocation(mProgId, uName.c_str());
	}

	void Shader::setAtribute(const std::string& attributeName)
	{
		glBindAttribLocation(mProgId, mNumAttributes++, attributeName.c_str());
	}
	void Shader::setUniform1b(std::string name, GLboolean value)
	{
		glUniform1i(getUniform(name), value);
	}
	void Shader::setUniform1i(std::string name, GLuint value)
	{
		glUniform1i(getUniform(name), value);
	}

	void Shader::setUniform1f(std::string name, float value)
	{
		glUniform1f(getUniform(name), value);
	}

	void Shader::setUniform1iv(std::string name, GLint* value, GLuint size)
	{
		glUniform1iv(getUniform(name), size, value);
	}

	void Shader::setUniform2f(std::string name, Vec2f value)
	{
		glUniform2f(getUniform(name), value.x, value.y);
	}

	void Shader::setUniform3f(std::string name, Vec3f value)
	{
		glUniform3f(getUniform(name), value.x, value.y, value.z);
	}

	void Shader::setUniform4f(std::string name, const Vec4f& value)
	{
		glUniform4f(getUniform(name), value.x, value.y, value.z, value.w);
	}

	void Shader::setUniformMat4(std::string name, const glm::mat4& value)
	{
		glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &value[0][0]);
	}

	void Shader::compileShader(uint32_t id, std::string shaderPath)
	{
		bool isShader = shaderPath.find("void main") != -1;

		std::string data = isShader ? shaderPath : FileIO::readFileAsString(shaderPath.c_str());

		char const* vertSource = data.c_str();

		glShaderSource(id, 1, &vertSource, nullptr);
		glCompileShader(id);

		if (!getCompileError(id, shaderPath))
		{
			compileError = true;
			return;
		}
	}

	bool Shader::getCompileError(uint32_t id, std::string shader)
	{
		int Result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &Result);

		if (Result == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			Logger::error("Error: %s\n", &(errorLog[0]));
			Logger::error("Fail  to compile %s\n", shader.c_str());
			return false;
		}

		return true;
	}

	void Shader::cleanUp(GLuint vertId, GLuint fragId)
	{
		glDeleteShader(vertId);
		glDeleteShader(fragId);
		glDeleteProgram(mProgId);
	}
} // namespace Plutus