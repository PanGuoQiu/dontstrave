#include "shader.h"

std::unordered_map<std::string, Shader*> Shader::m_ShaderMap;

// ���캯��
Shader::Shader()
{
}

// ��������
Shader::~Shader()
{
}

// ���һ򴴽�Shader
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

// ������ɫ���ļ�
void Shader::Parse(std::string shaderName)
{
	// ������ת��Ϊ�ļ�����
	const std::string vertexPath   = "shaders/" + shaderName + ".vs";
	const std::string fragmentPath = "shaders/" + shaderName + ".fs";
	const std::string geometryPath = "shaders/" + shaderName + ".gs";

	//��������/Ƭ��Դ�����·��
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile(geometryPath);

	//ȷ������������׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//��ȡ�ļ��Ļ������ݵ�����
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//�ر��ļ����
		vShaderFile.close();
		fShaderFile.close();
		//ת�������ַ�����
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//���������ɫ��·�����ڣ�������һ��������ɫ��
		if (gShaderFile.is_open())
		{
			std::cout << "������ɫ���ļ�����" << std::endl;
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

	// ������ɫ��
	const char* gShaderCode;
	if (geometryCode.length() == 0)
		gShaderCode = nullptr;
	else
		gShaderCode = geometryCode.c_str();
	CreateShader(vertexCode.c_str(), fragmentCode.c_str(), gShaderCode);
}

// ������ɫ��
void Shader::CreateShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode)
{
	//2.������ɫ��
	unsigned int vertex, fragment, geometry;
	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//��鶥����ɫ���Ƿ��д���
	CheckCompileErrors(vertex, "VERTEX");
	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//���Ƭ����ɫ���Ƿ��д���
	CheckCompileErrors(fragment, "FRAGMENT");
	//���������ɫ�����ڣ�����뼸����ɫ��
	if (gShaderCode != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		//��鼸����ɫ���Ƿ����
		CheckCompileErrors(geometry, "GEOMETRY");
	}

	//��ɫ����
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertex);
	glAttachShader(m_Id, fragment);
	if (gShaderCode != nullptr)
		glAttachShader(m_Id, geometry);
	glLinkProgram(m_Id);
	//�����ɫ����������Ƿ����
	CheckCompileErrors(m_Id, "PROGRAM");

	//�ͷ���ɫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (gShaderCode != nullptr)
		glDeleteShader(geometry);
}

// ���������
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