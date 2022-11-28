#ifndef LUAPARSER_H
#define LUAPARSER_H

#include <lua.hpp>
#include <iostream>

// lua解析器类
class LuaParser
{
public:
	// 析构函数
	~LuaParser();

	// 实例函数
	static LuaParser* LuaInstance();

	// 访问lua中的变量
	void ReadVariable();

	// 读取lua中的函数
	void LoadFunction();

	// 读取lua中的表
	void ReadTable();

	// 遍历table
	void Foreach();

private:
	// 构造函数
	LuaParser();

	// 初始化
	void Init();

	// 获得table中的项
	char* GetFeild(lua_State* L, const char* key);
	// table的下一个值
	int TableNext(lua_State* L, int i, char** k, char** v);

	// 实例和状态
	static LuaParser* m_pInstance;
	lua_State* m_pLuaState;
};

#endif //LUAPARSER_H
