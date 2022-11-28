#include "parser.h"

// ���캯��
Parser::Parser() : m_str(""), m_idx(0)
{
}

// ����xml�ļ�
bool Parser::LoadFile(const std::string& filename)
{
	// �ж϶�ȡ�ļ��Ƿ�ʧ��
	std::ifstream fin(filename);
	if (fin.fail())
	{
		return false;
	}

	// ��ȡxml�ļ���������
	std::stringstream ss;
	ss << fin.rdbuf();
	m_str = ss.str();
	m_idx = 0;

	return true;
}

// ����xml�ַ���
bool Parser::LoadString(const std::string& str)
{
	m_str = str;
	m_idx = 0;

	return true;
}

// �����ո�
void Parser::SkipWhiteSpace()
{
	while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
	{
		m_idx++;
	}
}

// ����XML�ļ�
Xml Parser::Parse()
{
	// ��������
	SkipWhiteSpace();
	if (m_str.compare(m_idx, 5, "<?xml") == 0)
	{
		if (!ParseDeclaration())
		{
			throw std::logic_error("Parse Declaration Error");
		}
	}

	// ����ע��
	SkipWhiteSpace();
	while (m_str.compare(m_idx, 4, "<!--") == 0)
	{
		if (ParseComment())
		{
			throw std::logic_error("Parse Comment Error");
		}
		SkipWhiteSpace();
	}

	// �����ڵ�
	if ((m_str[m_idx] == '<') && (isalpha(m_str[m_idx + 1]) || m_str[m_idx + 1] == '_'))
	{
		return ParseElement();
	}

	// ����������׳�����
	throw std::logic_error("Parse Element Error");
}

// ��������
bool Parser::ParseDeclaration()
{
	// �ж������Ŀ�ͷ
	if (m_str.compare(m_idx, 5, "<?xml") != 0)
	{
		return false;
	}

	m_idx += 5;
	// Ѱ�������Ľ�β���ж��Ƿ񳬳����ֵ
	size_t pos = m_str.find("?>", m_idx);
	if (pos == std::string::npos)
	{
		return false;
	}

	m_idx = pos + 2;
	return true;
}

// ����ע��
bool Parser::ParseComment()
{
	// �ж�ע�Ϳ�ͷ
	if (m_str.compare(m_idx, 4, "<!--") != 0)
		return false;

	// Ѱ��ע�͵Ľ�β�����ж��Ƿ񳬳����ֵ
	m_idx += 4;
	size_t pos = m_str.find("-->", m_idx);
	if (pos == std::string::npos)
		return false;

	m_idx = pos + 3;
	return true;
}

// �����ڵ�
Xml Parser::ParseElement()
{
	Xml elem;

	// �ڵ��<����
	m_idx++;
	SkipWhiteSpace();

	// �ڵ�����
	const std::string &name = ParseElementName();
	elem.name(name);

	// �ڵ�����
	while (m_str[m_idx] != '\0')
	{
		// �жϽڵ��Ƿ�Ϊ��β />
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
		else if (m_str[m_idx] == '>')                                   // �ж��Ƿ�Ϊ >
		{
			m_idx++;
			// ��ýڵ������
			std::string text = ParseElementText();
			if (text != "")
			{
				elem.text(text);
			}
		}
		else if (m_str[m_idx] == '<')                                   // �ڵ��β�� <
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
				// �ж���Ϊע��
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
			// �����ڵ������
			// ��
			std::string key = ParseElementAttrKey();
			SkipWhiteSpace();
			if (m_str[m_idx] != '=')
			{
				throw std::logic_error("xml element attr is error" + key);
			}

			// ֵ
			m_idx++;
			SkipWhiteSpace();
			std::string val = ParseElementAttrVal();
			elem.attr(key, val);
		}
	}

	return elem;
}

// �����ڵ�����
std::string Parser::ParseElementName()
{
	int pos = m_idx;
	// �ж��ַ��Ƿ�Ϊ��ĸ�����ж����ƵĿ�ͷ�ַ�������
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

// �����ڵ�����
std::string Parser::ParseElementText()
{
	int pos = m_idx;
	while (m_str[m_idx] != '<')
	{
		m_idx++;
	}

	return m_str.substr(pos, m_idx - pos);
}

// �����ڵ����Լ�
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

// �����ڵ�����ֵ
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