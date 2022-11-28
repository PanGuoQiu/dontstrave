#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"

// ��ɫ����
class Shader
{
public:
	// ���캯������������
	Shader();
	~Shader();

	// ���һ򴴽�Shader
	static Shader* Find(std::string shaderName);

	// ������ɫ���ļ�
	void Parse(std::string shaderName);

	// ������ɫ��
	void Active() { glUseProgram(m_Id); }

	// ������ɫ���б����ĵ�ַ���ı���ֵ
	// bool����
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
	}
	// int����
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
	}
	// float����
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
	}
	// vec2
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_Id, name.c_str()), x, y);
	}
	// vec3
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_Id, name.c_str()), x, y, z);
	}
	// vec4
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_Id, name.c_str()), x, y, z, w);
	}
	// mat2
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// mat3
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// mat4
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// ���������
	void CheckCompileErrors(GLuint shader, std::string type);

	// ������ɫ��
	void CreateShader(const char* vShader, const char* fShader, const char* gShader);

private:
	unsigned int m_Id;												// ��ɫ�����
	std::string m_ShaderName;										// ��ɫ������
	static std::unordered_map<std::string, Shader*> m_ShaderMap;	// �洢��ɫ��
};

#endif //SHADER_H
