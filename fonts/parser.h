#ifndef PARSER_H
#define PARSER_H

#include "xml.h"

// xml解析器类
class Parser
{
public:
	// 构造函数
	Parser();

	// 载入xml文件和xml字符串
	bool LoadFile(const std::string& filename);
	bool LoadString(const std::string& str);

	// 解析XML文件
	Xml Parse();

private:
	void SkipWhiteSpace();					// 跳过空格
	bool ParseDeclaration();				// 解析声明
	bool ParseComment();					// 解析注释
	Xml  ParseElement();					// 解析节点

	std::string ParseElementName();			// 解析节点名称
	std::string ParseElementText();			// 解析节点内容
	std::string ParseElementAttrKey();		// 解析节点属性键
	std::string ParseElementAttrVal();		// 解析节点属性值

private:
	std::string m_str;						// xml文件读取到m_str中
	int m_idx;								// 下标
};

#endif //PARSER_H
