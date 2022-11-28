#ifndef PARSER_H
#define PARSER_H

#include "xml.h"

// xml��������
class Parser
{
public:
	// ���캯��
	Parser();

	// ����xml�ļ���xml�ַ���
	bool LoadFile(const std::string& filename);
	bool LoadString(const std::string& str);

	// ����XML�ļ�
	Xml Parse();

private:
	void SkipWhiteSpace();					// �����ո�
	bool ParseDeclaration();				// ��������
	bool ParseComment();					// ����ע��
	Xml  ParseElement();					// �����ڵ�

	std::string ParseElementName();			// �����ڵ�����
	std::string ParseElementText();			// �����ڵ�����
	std::string ParseElementAttrKey();		// �����ڵ����Լ�
	std::string ParseElementAttrVal();		// �����ڵ�����ֵ

private:
	std::string m_str;						// xml�ļ���ȡ��m_str��
	int m_idx;								// �±�
};

#endif //PARSER_H
