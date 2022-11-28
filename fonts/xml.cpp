#include "xml.h"
#include "parser.h"

// 构造函数
Xml::Xml() : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
}

Xml::Xml(const char* name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
	m_name = new std::string(name);
}

Xml::Xml(const std::string& name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
	m_name = new std::string(name);
}

// 拷贝构造函数
Xml::Xml(const Xml& other)
{
	// 浅拷贝，直接复制指针地址
	m_name = other.m_name;
	m_text = other.m_text;
	m_attrs = other.m_attrs;
	m_child = other.m_child;
}

// 释放内存
void Xml::clear()
{
	// 节点名称
	if (m_name != nullptr)
	{
		delete m_name;
		m_name = nullptr;
	}

	// 节点内容
	if (m_text != nullptr)
	{
		delete m_text;
		m_text = nullptr;
	}

	// 节点属性
	if (m_attrs != nullptr)
	{
		delete m_attrs;
		m_attrs = nullptr;
	}

	// 子节点，需要考虑：子节点里面的子节点
	if (m_child != nullptr)
	{
		// 遍历所有子节点
		for (auto it = m_child->begin(); it != m_child->end(); ++it)
		{
			it->clear();
		}

		delete m_child;
		m_child = nullptr;
	}
}

// 获得节点名称
std::string Xml::name() const
{
	if (m_name == nullptr)
	{
		return "";
	}

	return (*m_name);
}

// 设置节点名称
void Xml::name(const std::string& name)
{
	if (m_name != nullptr)
	{
		delete m_name;
		m_name = nullptr;
	}

	m_name = new std::string(name);
}

// 获得节点内容
std::string Xml::text() const
{
	if (m_text == nullptr)
	{
		return "";
	}

	return (*m_text);
}

// 设置节点内容
void Xml::text(const std::string& text)
{
	if (m_text != nullptr)
	{
		delete m_text;
		m_text = nullptr;
	}

	m_text = new std::string(text);
}

// 获得节点属性
Value& Xml::attr(const std::string& key)
{
	if (m_attrs == nullptr)
	{
		m_attrs = new std::map<std::string, Value>();
	}

	return (*m_attrs)[key];
}

// 设置节点属性
void Xml::attr(const std::string& key, const Value& val)
{
	if (m_attrs == nullptr)
	{
		m_attrs = new std::map<std::string, Value>();
	}

	(*m_attrs)[key] = val;
}

// 序列化
std::string Xml::str() const
{
	if (m_name == nullptr)
	{
		return "";
	}

	std::stringstream os;
	// 节点头部
	os << "<";
	os << *m_name;
	// 判断节点是否有属性
	if (m_attrs != nullptr)
	{
		for (auto it = m_attrs->begin(); it != m_attrs->end(); ++it)
		{
			os << " " << it->first << "=\"" << (std::string)(it->second) << "\"";
		}
	}
	os << ">";

	// 判断是否有子节点
	if (m_child != nullptr)
	{
		for (auto it = m_child->begin(); it != m_child->end(); ++it)
		{
			os << it->str();
		}
	}

	// 判断是否有内容
	if (m_text != nullptr)
	{
		os << *m_text;
	}

	// 节点尾部
	os << "</" << *m_name << ">";

	// 返回序列化后的字符串
	return os.str();
}

// 添加子节点
void Xml::append(const Xml& child)
{
	// 判断子节点是否为空
	if (m_child == nullptr)
	{
		// 如果为空，则new一个
		m_child = new std::list<Xml>();
	}

	m_child->push_back(child);
}

// 通过下标索引的方式获得子节点
Xml& Xml::operator[](int index)
{
	if (index < 0)
		throw std::logic_error("索引值小于0");

	if (m_child == nullptr)
	{
		m_child = new std::list<Xml>();
	}

	// 遍历下标，找到index对应的Xml
	int size = m_child->size();
	if (index >= 0 && index < size)
	{
		auto it = m_child->begin();
		for (int i = 0; i < index; ++i)
		{
			it++;
		}

		return (*it);
	}

	// index > size, 则需要扩容
	if (index >= size)
	{
		for (int i = size; i <= index; i++)
		{
			m_child->push_back(Xml());
		}
	}

	// 返回最后一个节点
	return m_child->back();
}

// 根据节点名，返回节点
Xml& Xml::operator[](const char* name)
{
	return (*this)[std::string(name)];
}

Xml& Xml::operator[](const std::string& name)
{
	// 判断子节点是否为空
	if (m_child == nullptr)
	{
		// 如果为空，则创建
		m_child = new std::list<Xml>();
	}

	// 遍历子节点
	for (auto it = m_child->begin(); it != m_child->end(); it++)
	{
		if (it->name() == name)
		{
			return (*it);
		}
	}

	// 遍历后，没有找到，则创建
	m_child->push_back(Xml(name));
	return m_child->back();
}

// 通过下标删除子节点
void Xml::remove(int index)
{
	// 检验下标的合法性
	if (m_child == nullptr)
		return;

	int size = m_child->size();
	if (index < 0 || index >= size)
		return;

	// 遍历合法下标
	auto it = m_child->begin();
	for (int i = 0; i < index; i++)
	{
		it++;
	}

	// 释放节点内存
	it->clear();
	m_child->erase(it);
}

// 通过节点名称来删除子节点
void Xml::remove(const char* name)
{
	this->remove(std::string(name));
}

void Xml::remove(const std::string& name)
{
	if (m_child == nullptr)
		return;

	// 删除所有名称相同的节点
	for (auto it = m_child->begin(); it != m_child->end(); )
	{
		if (it->name() == name)
		{
			// 释放内存，并返回下一个迭代器节点
			it->clear();
			it = m_child->erase(it);
		}
		else
		{
			it++;
		}
	}
}

// 重载=运算符
Xml& Xml::operator=(const Xml& other)
{
	clear();
	m_name = other.m_name;
	m_text = other.m_text;
	m_attrs = other.m_attrs;
	m_child = other.m_child;

	return *this;
}

// 载入xml文件
bool Xml::load(const std::string& filename)
{
	Parser p;
	if (!p.LoadFile(filename))
	{
		std::cout << "载入xml文件失败" << std::endl;
		return false;
	}

	*this = p.Parse();
	return true;
}

// 保存为xml文件
bool Xml::save(const std::string& filename)
{
	std::ofstream fout(filename);
	if (fout.fail())
		return false;

	fout << str();
	fout.close();
	return true;
}

// xml解析
bool Xml::parse(const std::string& str)
{
	Parser p;
	if (!p.LoadString(str))
		return false;

	*this = p.Parse();
	return true;
}
