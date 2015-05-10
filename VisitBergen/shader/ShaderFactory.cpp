#include "ShaderFactory.h"

#include <vector>

#include "..\util\fileutil.h"

std::shared_ptr<ShaderProgram> ShaderFactory::makeShaderProgram(const std::string &shaderName)
{
	if (shaderName == "phongLighting" || true) // or true to always be true *for testing purposes
	{
		std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>();
		program->shaderProgram = initGLProgram(shaderName);

		if (program->shaderProgram == 0)
		{
			printf("Warning: ShaderFactory failed to initialize shader: (%s).\n", shaderName.c_str());
			return nullptr;
		}

		program->initAttribLocations();
		//program->attribLocations["vertex"] = glGetAttribLocation(program->shaderProgram, "vertex");
		//program->attribLocations["normal"] = glGetAttribLocation(program->shaderProgram, "normal");

		program->initTextureUniforms();
		program->initMaterialUniforms();
		program->initLightUniforms();
		program->initModelViewProjectionUniforms();

		GLState nullState;
		program->updateAllUniforms(nullState);

		printf("%s has been loaded as a proper ShaderProgram.\n", shaderName.c_str());

		return program;
	}

	return nullptr;
}

GLuint ShaderFactory::initGLProgram(const std::string &shaderProgram)
{
	GLuint glProgram = glCreateProgram();
	if (glProgram == 0)
	{
		GLenum error = glGetError();

		std::string errorMsg = "Could not create a new shader program. ";

		switch (error)
		{
			case GL_NO_ERROR:
				errorMsg += "No Error\n";
				break;
			case GL_INVALID_ENUM:
				errorMsg += "An unacceptable value is specified for an enumerated argument.\n";
				break;
			case GL_INVALID_VALUE:
				errorMsg += "A numeric argument is out of range.\n";
				break;
			case GL_INVALID_OPERATION:
				errorMsg += "The specified operation has been performed out of context, or is not allowed in the current state.\n";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorMsg += "The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE).\n";
				break;
			case GL_OUT_OF_MEMORY:
				errorMsg += "There is not enough memory left to execute the command.\n";
				break;
			case GL_STACK_UNDERFLOW:
				errorMsg += "An attempt has been made to perform an operation that would cause an internal stack to underflow.\n";
				break;
			case GL_STACK_OVERFLOW:
				errorMsg += "An attempt has been made to perform an operation that would cause an internal stack to overflow.\n";
				break;
			default:
				errorMsg += "Reason unknown (default).\n";
		}


		printf("Error: ShaderFactory: Program cannot be created (%s).\n", shaderProgram.c_str());
		printf("%s\n", errorMsg.c_str());
		return 0;
	}

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (vertShader == 0 || fragShader == 0)
	{
		printf("Error: ShaderFacctory: Unable to create shader objects.");
		return 0;
	}

	std::string vertexShaderSourceCodePath = "shader//" + shaderProgram + ".v.glsl";
	std::string vertexShaderSourceCode = readTextFile(vertexShaderSourceCodePath);
	const char* vertexShaderSource = vertexShaderSourceCode.c_str();
	int vsLength = static_cast<int>(vertexShaderSourceCode.length());
	if (vsLength == 0)
	{
		printf("Error: ShaderFactory: Received empty vertex shader.\n");
		return 0;
	}

	std::string fragmentShaderSourceCode = readTextFile("shader//" + shaderProgram + ".f.glsl");
	const char* fragmentShaderSource = fragmentShaderSourceCode.c_str();
	int fsLength = static_cast<int>(fragmentShaderSourceCode.length());
	if (fsLength == 0)
	{
		printf("Error: ShaderFactory::initGLProgram received empty fragment shader.\n");
		return 0;
	}

	glShaderSource(vertShader, 1, &vertexShaderSource, &vsLength);
	glShaderSource(fragShader, 1, &fragmentShaderSource, &fsLength);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint success = 0;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// get length of InfoLog, create vector with length and put error into vector
		GLint infoLength = 0;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetShaderInfoLog(vertShader, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error compiling vertex shadeer: \n %s \n", errorInfoLog.c_str());

		// don't leak shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		
		return 0;
	}

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Get length of InfoLog, create vector with length and put errorLog into vector
		GLint infoLength = 0;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetShaderInfoLog(fragShader, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error compiling fragment shader:\n %s \n", errorInfoLog.c_str());

		// Don't leak shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return 0;
	}

	// Attach shaders to program and link it
	glAttachShader(glProgram, vertShader);
	glAttachShader(glProgram, fragShader);
	glLinkProgram(glProgram);

	// Check for linking error
	glGetProgramiv(glProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetProgramInfoLog(glProgram, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error compiling fragment shader: \n %s \n", errorInfoLog.c_str());

		// Don't leak program or shaders
		glDeleteProgram(glProgram);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return 0;
	}

	// Successful linking, now detach and delete shaders
	glDetachShader(glProgram, vertShader);
	glDetachShader(glProgram, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// Validate program
	glValidateProgram(glProgram);

	// Check for validation errors
	glGetProgramiv(glProgram, GL_VALIDATE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetProgramInfoLog(glProgram, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error: Invalid shaderprogram: \n %s \n", errorInfoLog.c_str());

		return 0;
	}

	// Program was successfully linked and validated!
	return glProgram;
}