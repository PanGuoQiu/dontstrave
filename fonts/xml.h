#ifndef XML_H
#define XML_H

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <fstream>
#include <sstream>

// 变量类
class Value
{
public:
	// 构造函数和析构函数
	Value() {}
	Value(bool value) { *this = value; }
	Value(int value) { *this = value; }
	Value(double value) { *this = value; }
	Value(const char* value) : m_value(value) {}
	Value(const std::string& value) : m_value(value) {}
	~Value() {}

	// 重载=运算符
	Value& operator=(bool value) { m_value = value ? "true" : "false"; return *this; }
	Value& operator=(int value) { std::stringstream ss; ss << value; m_value = ss.str(); return *this; }
	Value& operator=(double value) { std::stringstream ss; ss << value; m_value = ss.str(); return *this; }
	Value& operator=(const char* value) { m_value = value; return *this; }
	Value& operator=(const std::string& value) { m_value = value; return *this; }
	Value& operator=(const Value& value) { m_value = value.m_value; return *this; }

	// 重载==和!=运算符
	bool operator==(const Value& other) { return (m_value == other.m_value); }
	bool operator!=(const Value& other) { return (m_value != other.m_value); }

	// 变量类转换为其他类型
	operator bool() { if (m_value == "true") return true; else if (m_value == "false") return false; }
	operator int() { return std::atoi(m_value.c_str()); }
	operator double() { return std::atof(m_value.c_str()); }
	operator std::string() { return m_value; }

private:
	std::string m_value;
};

// XML类
class Xml
{
public:
	// 构造函数
	Xml();
	Xml(const char* name);
	Xml(const std::string& name);
	// 拷贝构造函数
	Xml(const Xml& other);

	// 释放内存
	void clear();

	// 获得节点名称和设置节点名称
	std::string name() const;
	void name(const std::string& name);

	// 获得节点内容和设置节点内容
	std::string text() const;
	void text(const std::string& text);

	// 获得节点属性和设置节点属性
	Value& attr(const std::string& key);
	void attr(const std::string& key, const Value& val);

	// 序列化：将C++类转换为xml文件
	std::string str() const;

	// 添加子节点
	void append(const Xml& child);

	// 通过下标索引获得子节点
	Xml& operator[](int index);
	// 根据节点名称，返回节点
	Xml& operator[](const char* name);
	Xml& operator[](const std::string& name);

	// 删除节点
	void remove(int index);
	void remove(const char* name);
	void remove(const std::string& name);

	// 重载=运算符
	Xml& operator=(const Xml& other);

	// 定义一个迭代器，遍历子节点
	typedef std::list<Xml>::iterator iterator;
	iterator begin() { return m_child->begin(); }
	iterator end() { return m_child->end(); }
	iterator erase(iterator it)
	{
		it->clear();
		return m_child->erase(it);
	}

	// 获得子节点的个数
	int size() const { return m_child->size(); }

	// 载入xml文件
	bool load(const std::string& filename);			// 载入xml文件
	bool save(const std::string& filename);			// 保存xml文件
	bool parse(const std::string& str);				// 解析xml字符串

private:
	std::string* m_name;							// 节点名称
	std::string* m_text;							// 节点内容
	std::map<std::string, Value>* m_attrs;			// 节点属性
	std::list<Xml>* m_child;						// 子节点
};

#endif //XML_H
