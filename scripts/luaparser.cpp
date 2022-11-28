#include "luaparser.h"

// 初始化lua实例
LuaParser* LuaParser::m_pInstance = nullptr;

// 构造函数
LuaParser::LuaParser()
{
}

// 析构函数
LuaParser::~LuaParser()
{
	if (m_pInstance)
	{
		lua_close(m_pLuaState);
		m_pLuaState = nullptr;
	}
}

// 实例化
LuaParser* LuaParser::LuaInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new LuaParser();
		m_pInstance->Init();
	}

	return m_pInstance;
}

// lua访问C++函数
static int showdd(lua_State* L)
{
	// 压入栈中的变量值
	lua_pushstring(L, "push string to Lua");

	// 返回压入栈中的个数
	return 1;
}

// 初始化
void LuaParser::Init()
{
	// 创建一个lua状态指针
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);

	// 把C++函数地址压入栈中，并设置地址名称
	lua_pushcfunction(m_pLuaState, showdd);
	lua_setglobal(m_pLuaState, "show");
}

// 访问lua中的变量
void LuaParser::ReadVariable()
{
	luaL_dofile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "执行lua文件失败" << std::endl;
	}

	// 获得lua中的全局变量
	//lua_getglobal(m_pLuaState, "width");
	//lua_getglobal(m_pLuaState, "height");

	//// 将获得变量转换为C使用的变量值()
	//unsigned int width = lua_tonumber(m_pLuaState, -2);
	//unsigned int height = lua_tonumber(m_pLuaState, -1);
	//std::cout << "width: " << width << std::endl;
	//std::cout << "height: " << height << std::endl;

	// 访问一个变量，弹出一个变量，这样不会乱
	lua_getglobal(m_pLuaState, "width");
	unsigned int width = (unsigned int)lua_tonumber(m_pLuaState, -1);
	std::cout << "width: " << width << std::endl;
	lua_pop(m_pLuaState, 1);

	lua_getglobal(m_pLuaState, "height");
	unsigned int height = (unsigned int)lua_tonumber(m_pLuaState, -1);
	std::cout << "height: " << height << std::endl;
	lua_pop(m_pLuaState, 1);
}

// 访问lua中的函数
void LuaParser::LoadFunction()
{
	luaL_dofile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "执行lua文件失败" << std::endl;
	}

	// 把函数地址压入栈中，然后，再压入参数()
	lua_getglobal(m_pLuaState, "add");
	lua_pushnumber(m_pLuaState, 3);
	lua_pushnumber(m_pLuaState, 4);

	// 执行栈中函数地址 两个参数、一个返回值、一个错误值
	err_rlt = lua_pcall(m_pLuaState, 2, 1, -1);
	if (err_rlt == -1)
	{
		std::cout << "执行lua中的函数失败" << std::endl;
	}

	// 再从栈中获得返回值
	err_rlt = (int)lua_tointeger(m_pLuaState, -1);
	std::cout << "add: " << err_rlt << std::endl;
}

// 获得table中的值
char* LuaParser::GetFeild(lua_State* L, const char* key)
{
	char* rlt = nullptr;

	// key压入栈中，作为参数获得table的值
	lua_pushstring(L, key);
	// 根据key值获取table中的值，和key形成键值对
	lua_gettable(L, -2);

	// 栈：value、key、table地址
	// 判断value是否为string
	if (lua_isstring(L, -1))
	{
		// 获得value值
		rlt = (char*)lua_tostring(L, -1);

		// 栈：key、table地址
		// 弹出key，这样下一次栈顶还是为table地址
		lua_pop(L, 1);

		return rlt;
	}

	return "wrong";
}

// 访问lua中的表
void LuaParser::ReadTable()
{
	// 载入lua脚本
	luaL_dofile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "读取lua文件错误" << std::endl;
	}

	// 把名称的地址压入栈顶，并判断是否为表
	lua_getglobal(m_pLuaState, "application");
	if (lua_istable(m_pLuaState, -1))
	{
		// 如果为true，则是table
		std::cout << "application is table" << std::endl;

		// 获得table中的值
		char* width = GetFeild(m_pLuaState, "width");
		char* height = GetFeild(m_pLuaState, "height");

		// 获得table中的值
		std::cout << "width is: " << width << std::endl;
		std::cout << "height is: " << height << std::endl;
	}
}

// 获得table的下一个值
int LuaParser::TableNext(lua_State* L, int i, char** k, char** v)
{
	// 判断是否还有下一个值
	if (lua_next(L, i) != 0)
	{
		// 栈：value、key
		// 原理：每取出一key的值，就弹出一个table值，使下标永远指向栈顶
		*k = (char*)lua_tostring(L, -2);
		*v = (char*)lua_tostring(L, -1);
		lua_pop(L, i);

		return 1;
	}
	else
	{
		return 0;
	}
}

// 遍历table
void LuaParser::Foreach()
{
	lua_State* L;
	int idx;
	char* k = nullptr;
	char* v = nullptr;

	// 载入脚本文件
	luaL_loadfile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "读取lua文件错误" << std::endl;
	}

	// 把table地址压入栈中
	lua_getglobal(m_pLuaState, "application");
	idx = lua_gettop(m_pLuaState);									// 获得指向栈顶的下标
	lua_pushnil(m_pLuaState);										// 压入一个nil

																	// 遍历
	while (TableNext(m_pLuaState, 1, &k, &v) != 0)
	{
		std::cout << "key: " << k << ", value: " << v << std::endl;
	}

	lua_close(m_pLuaState);
}