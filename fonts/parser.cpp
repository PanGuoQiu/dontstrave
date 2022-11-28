#include "parser.h"

// 构造函数
Parser::Parser() : m_str(""), m_idx(0)
{
}

// 载入xml文件
bool Parser::LoadFile(const std::string& filename)
{
	// 判断读取文件是否失败
	std::ifstream fin(filename);
	if (fin.fail())
	{
		return false;
	}

	// 读取xml文件到缓存中
	std::stringstream ss;
	ss << fin.rdbuf();
	m_str = ss.str();
	m_idx = 0;

	return true;
}

// 载入xml字符串
bool Parser::LoadString(const std::string& str)
{
	m_str = str;
	m_idx = 0;

	return true;
}

// 跳过空格
void Parser::SkipWhiteSpace()
{
	while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
	{
		m_idx++;
	}
}

// 解析XML文件
Xml Parser::Parse()
{
	// 解析声明
	SkipWhiteSpace();
	if (m_str.compare(m_idx, 5, "<?xml") == 0)
	{
		if (!ParseDeclaration())
		{
			throw std::logic_error("Parse Declaration Error");
		}
	}

	// 解析注释
	SkipWhiteSpace();
	while (m_str.compare(m_idx, 4, "<!--") == 0)
	{
		if (ParseComment())
		{
			throw std::logic_error("Parse Comment Error");
		}
		SkipWhiteSpace();
	}

	// 解析节点
	if ((m_str[m_idx] == '<') && (isalpha(m_str[m_idx + 1]) || m_str[m_idx + 1] == '_'))
	{
		return ParseElement();
	}

	// 其他情况，抛出错误
	throw std::logic_error("Parse Element Error");
}

// 解析声明
bool Parser::ParseDeclaration()
{
	// 判断声明的开头
	if (m_str.compare(m_idx, 5, "<?xml") != 0)
	{
		return false;
	}

	m_idx += 5;
	// 寻找声明的结尾，判断是否超出最大值
	size_t pos = m_str.find("?>", m_idx);
	if (pos == std::string::npos)
	{
		return false;
	}

	m_idx = pos + 2;
	return true;
}

// 解析注释
bool Parser::ParseComment()
{
	// 判断注释开头
	if (m_str.compare(m_idx, 4, "<!--") != 0)
		return false;

	// 寻找注释的结尾，并判断是否超出最大值
	m_idx += 4;
	size_t pos = m_str.find("-->", m_idx);
	if (pos == std::string::npos)
		return false;

	m_idx = pos + 3;
	return true;
}

// 解析节点
Xml Parser::ParseElement()
{
	Xml elem;

	// 节点的<括号
	m_idx++;
	SkipWhiteSpace();

	// 节点名称
	const std::string &name = ParseElementName();
	elem.name(name);

	// 节点属性
	while (m_str[m_idx] != '\0')
	{
		// 判断节点是否为结尾 />
		SkipWhiteSpace();
		if (m_str[m_idx] == '/')
		{
			if (m_str[m_idx + 1] == '>')
			{
				m_idx += 2;
				break;
			}
			else
			{
				throw std::logic_error("xml empty element is error");
			}
		}
		else if (m_str[m_idx] == '>')                                   // 判断是否为 >
		{
			m_idx++;
			// 获得节点的内容
			std::string text = ParseElementText();
			if (text != "")
			{
				elem.text(text);
			}
		}
		else if (m_str[m_idx] == '<')                                   // 节点结尾的 <
		{
			if (m_str[m_idx + 1] == '/')
			{
				// find the end tag
				std::string end_tag = "</" + name + ">";
				size_t pos = m_str.find(end_tag, m_idx);
				if (pos == std::string::npos)
				{
					throw std::logic_error("xml element " + name + " end tag not found");
				}

				m_idx = (pos + end_tag.size());
				break;
			}
			else if (m_str.compare(m_idx, 4, "<!--") == 0)
			{
				// 判断是为注释
				if (!ParseComment())
				{
					throw std::logic_error("xml comment is error");
				}
			}
			else
			{
				// parse child element
				elem.append(ParseElement());
			}
		}
		else
		{
			// 解析节点的属性
			// 键
			std::string key = ParseElementAttrKey();
			SkipWhiteSpace();
			if (m_str[m_idx] != '=')
			{
				throw std::logic_error("xml element attr is error" + key);
			}

			// 值
			m_idx++;
			SkipWhiteSpace();
			std::string val = ParseElementAttrVal();
			elem.attr(key, val);
		}
	}

	return elem;
}

// 解析节点名称
std::string Parser::ParseElementName()
{
	int pos = m_idx;
	// 判断字符是否为字母，即判断名称的开头字符的类型
	if (isalpha(m_str[m_idx]) || (m_str[m_idx] == '_'))
	{
		m_idx++;
		while (isalnum(m_str[m_idx]) || (m_str[m_idx] == '_') || (m_str[m_idx] == '-') || (m_str[m_idx] == ':') || (m_str[m_idx] == '.'))
		{
			m_idx++;
		}
	}

	return m_str.substr(pos, m_idx - pos);
}

// 解析节点内容
std::string Parser::ParseElementText()
{
	int pos = m_idx;
	while (m_str[m_idx] != '<')
	{
		m_idx++;
	}

	return m_str.substr(pos, m_idx - pos);
}

// 解析节点属性键
std::string Parser::ParseElementAttrKey()
{
	int pos = m_idx;
	if (isalpha(m_str[m_idx]) || (m_str[m_idx] == '_'))
	{
		m_idx++;
		while (isalnum(m_str[m_idx]) || (m_str[m_idx] == '_') || (m_str[m_idx] == '-') || (m_str[m_idx] == ':'))
		{
			m_idx++;
		}
	}

	return m_str.substr(pos, m_idx - pos);
}

// 解析节点属性值
std::string Parser::ParseElementAttrVal()
{
	if (m_str[m_idx] != '"')
	{
		throw std::logic_error("xml element attr value should be in double quotes");
	}

	m_idx++;
	int pos = m_idx;
	while (m_str[m_idx] != '"')
	{
		m_idx++;
	}

	m_idx++;
	return m_str.substr(pos, m_idx - pos - 1);
}