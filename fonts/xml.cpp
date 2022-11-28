#include "xml.h"
#include "parser.h"

// ���캯��
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

// �������캯��
Xml::Xml(const Xml& other)
{
	// ǳ������ֱ�Ӹ���ָ���ַ
	m_name = other.m_name;
	m_text = other.m_text;
	m_attrs = other.m_attrs;
	m_child = other.m_child;
}

// �ͷ��ڴ�
void Xml::clear()
{
	// �ڵ�����
	if (m_name != nullptr)
	{
		delete m_name;
		m_name = nullptr;
	}

	// �ڵ�����
	if (m_text != nullptr)
	{
		delete m_text;
		m_text = nullptr;
	}

	// �ڵ�����
	if (m_attrs != nullptr)
	{
		delete m_attrs;
		m_attrs = nullptr;
	}

	// �ӽڵ㣬��Ҫ���ǣ��ӽڵ�������ӽڵ�
	if (m_child != nullptr)
	{
		// ���������ӽڵ�
		for (auto it = m_child->begin(); it != m_child->end(); ++it)
		{
			it->clear();
		}

		delete m_child;
		m_child = nullptr;
	}
}

// ��ýڵ�����
std::string Xml::name() const
{
	if (m_name == nullptr)
	{
		return "";
	}

	return (*m_name);
}

// ���ýڵ�����
void Xml::name(const std::string& name)
{
	if (m_name != nullptr)
	{
		delete m_name;
		m_name = nullptr;
	}

	m_name = new std::string(name);
}

// ��ýڵ�����
std::string Xml::text() const
{
	if (m_text == nullptr)
	{
		return "";
	}

	return (*m_text);
}

// ���ýڵ�����
void Xml::text(const std::string& text)
{
	if (m_text != nullptr)
	{
		delete m_text;
		m_text = nullptr;
	}

	m_text = new std::string(text);
}

// ��ýڵ�����
Value& Xml::attr(const std::string& key)
{
	if (m_attrs == nullptr)
	{
		m_attrs = new std::map<std::string, Value>();
	}

	return (*m_attrs)[key];
}

// ���ýڵ�����
void Xml::attr(const std::string& key, const Value& val)
{
	if (m_attrs == nullptr)
	{
		m_attrs = new std::map<std::string, Value>();
	}

	(*m_attrs)[key] = val;
}

// ���л�
std::string Xml::str() const
{
	if (m_name == nullptr)
	{
		return "";
	}

	std::stringstream os;
	// �ڵ�ͷ��
	os << "<";
	os << *m_name;
	// �жϽڵ��Ƿ�������
	if (m_attrs != nullptr)
	{
		for (auto it = m_attrs->begin(); it != m_attrs->end(); ++it)
		{
			os << " " << it->first << "=\"" << (std::string)(it->second) << "\"";
		}
	}
	os << ">";

	// �ж��Ƿ����ӽڵ�
	if (m_child != nullptr)
	{
		for (auto it = m_child->begin(); it != m_child->end(); ++it)
		{
			os << it->str();
		}
	}

	// �ж��Ƿ�������
	if (m_text != nullptr)
	{
		os << *m_text;
	}

	// �ڵ�β��
	os << "</" << *m_name << ">";

	// �������л�����ַ���
	return os.str();
}

// ����ӽڵ�
void Xml::append(const Xml& child)
{
	// �ж��ӽڵ��Ƿ�Ϊ��
	if (m_child == nullptr)
	{
		// ���Ϊ�գ���newһ��
		m_child = new std::list<Xml>();
	}

	m_child->push_back(child);
}

// ͨ���±������ķ�ʽ����ӽڵ�
Xml& Xml::operator[](int index)
{
	if (index < 0)
		throw std::logic_error("����ֵС��0");

	if (m_child == nullptr)
	{
		m_child = new std::list<Xml>();
	}

	// �����±꣬�ҵ�index��Ӧ��Xml
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

	// index > size, ����Ҫ����
	if (index >= size)
	{
		for (int i = size; i <= index; i++)
		{
			m_child->push_back(Xml());
		}
	}

	// �������һ���ڵ�
	return m_child->back();
}

// ���ݽڵ��������ؽڵ�
Xml& Xml::operator[](const char* name)
{
	return (*this)[std::string(name)];
}

Xml& Xml::operator[](const std::string& name)
{
	// �ж��ӽڵ��Ƿ�Ϊ��
	if (m_child == nullptr)
	{
		// ���Ϊ�գ��򴴽�
		m_child = new std::list<Xml>();
	}

	// �����ӽڵ�
	for (auto it = m_child->begin(); it != m_child->end(); it++)
	{
		if (it->name() == name)
		{
			return (*it);
		}
	}

	// ������û���ҵ����򴴽�
	m_child->push_back(Xml(name));
	return m_child->back();
}

// ͨ���±�ɾ���ӽڵ�
void Xml::remove(int index)
{
	// �����±�ĺϷ���
	if (m_child == nullptr)
		return;

	int size = m_child->size();
	if (index < 0 || index >= size)
		return;

	// �����Ϸ��±�
	auto it = m_child->begin();
	for (int i = 0; i < index; i++)
	{
		it++;
	}

	// �ͷŽڵ��ڴ�
	it->clear();
	m_child->erase(it);
}

// ͨ���ڵ�������ɾ���ӽڵ�
void Xml::remove(const char* name)
{
	this->remove(std::string(name));
}

void Xml::remove(const std::string& name)
{
	if (m_child == nullptr)
		return;

	// ɾ������������ͬ�Ľڵ�
	for (auto it = m_child->begin(); it != m_child->end(); )
	{
		if (it->name() == name)
		{
			// �ͷ��ڴ棬��������һ���������ڵ�
			it->clear();
			it = m_child->erase(it);
		}
		else
		{
			it++;
		}
	}
}

// ����=�����
Xml& Xml::operator=(const Xml& other)
{
	clear();
	m_name = other.m_name;
	m_text = other.m_text;
	m_attrs = other.m_attrs;
	m_child = other.m_child;

	return *this;
}

// ����xml�ļ�
bool Xml::load(const std::string& filename)
{
	Parser p;
	if (!p.LoadFile(filename))
	{
		std::cout << "����xml�ļ�ʧ��" << std::endl;
		return false;
	}

	*this = p.Parse();
	return true;
}

// ����Ϊxml�ļ�
bool Xml::save(const std::string& filename)
{
	std::ofstream fout(filename);
	if (fout.fail())
		return false;

	fout << str();
	fout.close();
	return true;
}

// xml����
bool Xml::parse(const std::string& str)
{
	Parser p;
	if (!p.LoadString(str))
		return false;

	*this = p.Parse();
	return true;
}
