#ifndef LUAPARSER_H
#define LUAPARSER_H

#include <lua.hpp>
#include <iostream>

// lua��������
class LuaParser
{
public:
	// ��������
	~LuaParser();

	// ʵ������
	static LuaParser* LuaInstance();

	// ����lua�еı���
	void ReadVariable();

	// ��ȡlua�еĺ���
	void LoadFunction();

	// ��ȡlua�еı�
	void ReadTable();

	// ����table
	void Foreach();

private:
	// ���캯��
	LuaParser();

	// ��ʼ��
	void Init();

	// ���table�е���
	char* GetFeild(lua_State* L, const char* key);
	// table����һ��ֵ
	int TableNext(lua_State* L, int i, char** k, char** v);

	// ʵ����״̬
	static LuaParser* m_pInstance;
	lua_State* m_pLuaState;
};

#endif //LUAPARSER_H
