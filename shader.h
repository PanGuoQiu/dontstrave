#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"

// 着色器类
class Shader
{
public:
	// 构造函数和析构函数
	Shader();
	~Shader();

	// 查找或创建Shader
	static Shader* Find(std::string shaderName);

	// 解析着色器文件
	void Parse(std::string shaderName);

	// 激活着色器
	void Active() { glUseProgram(m_Id); }

	// 查找着色器中变量的地址并改变其值
	// bool类型
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
	}
	// int类型
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
	}
	// float类型
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
	// 检查编译错误
	void CheckCompileErrors(GLuint shader, std::string type);

	// 创建着色器
	void CreateShader(const char* vShader, const char* fShader, const char* gShader);

private:
	unsigned int m_Id;												// 着色器句柄
	std::string m_ShaderName;										// 着色器名称
	static std::unordered_map<std::string, Shader*> m_ShaderMap;	// 存储着色器
};

#endif //SHADER_H
