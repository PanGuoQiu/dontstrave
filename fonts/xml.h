#ifndef XML_H
#define XML_H

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <fstream>
#include <sstream>

// ������
class Value
{
public:
	// ���캯������������
	Value() {}
	Value(bool value) { *this = value; }
	Value(int value) { *this = value; }
	Value(double value) { *this = value; }
	Value(const char* value) : m_value(value) {}
	Value(const std::string& value) : m_value(value) {}
	~Value() {}

	// ����=�����
	Value& operator=(bool value) { m_value = value ? "true" : "false"; return *this; }
	Value& operator=(int value) { std::stringstream ss; ss << value; m_value = ss.str(); return *this; }
	Value& operator=(double value) { std::stringstream ss; ss << value; m_value = ss.str(); return *this; }
	Value& operator=(const char* value) { m_value = value; return *this; }
	Value& operator=(const std::string& value) { m_value = value; return *this; }
	Value& operator=(const Value& value) { m_value = value.m_value; return *this; }

	// ����==��!=�����
	bool operator==(const Value& other) { return (m_value == other.m_value); }
	bool operator!=(const Value& other) { return (m_value != other.m_value); }

	// ������ת��Ϊ��������
	operator bool() { if (m_value == "true") return true; else if (m_value == "false") return false; }
	operator int() { return std::atoi(m_value.c_str()); }
	operator double() { return std::atof(m_value.c_str()); }
	operator std::string() { return m_value; }

private:
	std::string m_value;
};

// XML��
class Xml
{
public:
	// ���캯��
	Xml();
	Xml(const char* name);
	Xml(const std::string& name);
	// �������캯��
	Xml(const Xml& other);

	// �ͷ��ڴ�
	void clear();

	// ��ýڵ����ƺ����ýڵ�����
	std::string name() const;
	void name(const std::string& name);

	// ��ýڵ����ݺ����ýڵ�����
	std::string text() const;
	void text(const std::string& text);

	// ��ýڵ����Ժ����ýڵ�����
	Value& attr(const std::string& key);
	void attr(const std::string& key, const Value& val);

	// ���л�����C++��ת��Ϊxml�ļ�
	std::string str() const;

	// ����ӽڵ�
	void append(const Xml& child);

	// ͨ���±���������ӽڵ�
	Xml& operator[](int index);
	// ���ݽڵ����ƣ����ؽڵ�
	Xml& operator[](const char* name);
	Xml& operator[](const std::string& name);

	// ɾ���ڵ�
	void remove(int index);
	void remove(const char* name);
	void remove(const std::string& name);

	// ����=�����
	Xml& operator=(const Xml& other);

	// ����һ���������������ӽڵ�
	typedef std::list<Xml>::iterator iterator;
	iterator begin() { return m_child->begin(); }
	iterator end() { return m_child->end(); }
	iterator erase(iterator it)
	{
		it->clear();
		return m_child->erase(it);
	}

	// ����ӽڵ�ĸ���
	int size() const { return m_child->size(); }

	// ����xml�ļ�
	bool load(const std::string& filename);			// ����xml�ļ�
	bool save(const std::string& filename);			// ����xml�ļ�
	bool parse(const std::string& str);				// ����xml�ַ���

private:
	std::string* m_name;							// �ڵ�����
	std::string* m_text;							// �ڵ�����
	std::map<std::string, Value>* m_attrs;			// �ڵ�����
	std::list<Xml>* m_child;						// �ӽڵ�
};

#endif //XML_H
