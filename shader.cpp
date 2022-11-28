#include "shader.h"

std::unordered_map<std::string, Shader*> Shader::m_ShaderMap;

// 构造函数
Shader::Shader()
{
}

// 析构函数
Shader::~Shader()
{
}

// 查找或创建Shader
Shader* Shader::Find(std::string shaderName)
{
	std::unordered_map<std::string, Shader*>::iterator iter = m_ShaderMap.find(shaderName);
	if (iter != m_ShaderMap.end())
	{
		return iter->second;
	}

	Shader* shader = new Shader();
	shader->Parse(shaderName);

	m_ShaderMap.insert(std::pair<std::string, Shader*>(shaderName, shader));

	return shader;
}

// 解析着色器文件
void Shader::Parse(std::string shaderName)
{
	// 将名称转换为文件名称
	const std::string vertexPath   = "shaders/" + shaderName + ".vs";
	const std::string fragmentPath = "shaders/" + shaderName + ".fs";
	const std::string geometryPath = "shaders/" + shaderName + ".gs";

	//检索顶点/片段源代码的路径
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile(geometryPath);

	//确保流对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓存内容到流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件句柄
		vShaderFile.close();
		fShaderFile.close();
		//转换流到字符串中
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//如果几何着色器路径存在，则载入一个几何着色器
		if (gShaderFile.is_open())
		{
			std::cout << "几何着色器文件存在" << std::endl;
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
		else
		{
			geometryCode = "";
		}
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// 创建着色器
	const char* gShaderCode;
	if (geometryCode.length() == 0)
		gShaderCode = nullptr;
	else
		gShaderCode = geometryCode.c_str();
	CreateShader(vertexCode.c_str(), fragmentCode.c_str(), gShaderCode);
}

// 创建着色器
void Shader::CreateShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode)
{
	//2.编译着色器
	unsigned int vertex, fragment, geometry;
	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//检查顶点着色器是否有错误
	CheckCompileErrors(vertex, "VERTEX");
	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//检查片段着色器是否有错误
	CheckCompileErrors(fragment, "FRAGMENT");
	//如果几何着色器存在，则编译几何着色器
	if (gShaderCode != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		//检查几何着色器是否错误
		CheckCompileErrors(geometry, "GEOMETRY");
	}

	//着色程序
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertex);
	glAttachShader(m_Id, fragment);
	if (gShaderCode != nullptr)
		glAttachShader(m_Id, geometry);
	glLinkProgram(m_Id);
	//检查着色器程序编译是否错误
	CheckCompileErrors(m_Id, "PROGRAM");

	//释放着色器
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (gShaderCode != nullptr)
		glDeleteShader(geometry);
}

// 检查编译错误
void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl;
			std::cout << infoLog << std::endl;
			std::cout << "-------------------------------------------------" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl;
			std::cout << infoLog << std::endl;
			std::cout << "-------------------------------------------------" << std::endl;
		}
	}
}