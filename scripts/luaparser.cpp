#include "luaparser.h"

// ��ʼ��luaʵ��
LuaParser* LuaParser::m_pInstance = nullptr;

// ���캯��
LuaParser::LuaParser()
{
}

// ��������
LuaParser::~LuaParser()
{
	if (m_pInstance)
	{
		lua_close(m_pLuaState);
		m_pLuaState = nullptr;
	}
}

// ʵ����
LuaParser* LuaParser::LuaInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new LuaParser();
		m_pInstance->Init();
	}

	return m_pInstance;
}

// lua����C++����
static int showdd(lua_State* L)
{
	// ѹ��ջ�еı���ֵ
	lua_pushstring(L, "push string to Lua");

	// ����ѹ��ջ�еĸ���
	return 1;
}

// ��ʼ��
void LuaParser::Init()
{
	// ����һ��lua״ָ̬��
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);

	// ��C++������ַѹ��ջ�У������õ�ַ����
	lua_pushcfunction(m_pLuaState, showdd);
	lua_setglobal(m_pLuaState, "show");
}

// ����lua�еı���
void LuaParser::ReadVariable()
{
	luaL_dofile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "ִ��lua�ļ�ʧ��" << std::endl;
	}

	// ���lua�е�ȫ�ֱ���
	//lua_getglobal(m_pLuaState, "width");
	//lua_getglobal(m_pLuaState, "height");

	//// ����ñ���ת��ΪCʹ�õı���ֵ()
	//unsigned int width = lua_tonumber(m_pLuaState, -2);
	//unsigned int height = lua_tonumber(m_pLuaState, -1);
	//std::cout << "width: " << width << std::endl;
	//std::cout << "height: " << height << std::endl;

	// ����һ������������һ������������������
	lua_getglobal(m_pLuaState, "width");
	unsigned int width = (unsigned int)lua_tonumber(m_pLuaState, -1);
	std::cout << "width: " << width << std::endl;
	lua_pop(m_pLuaState, 1);

	lua_getglobal(m_pLuaState, "height");
	unsigned int height = (unsigned int)lua_tonumber(m_pLuaState, -1);
	std::cout << "height: " << height << std::endl;
	lua_pop(m_pLuaState, 1);
}

// ����lua�еĺ���
void LuaParser::LoadFunction()
{
	luaL_dofile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "ִ��lua�ļ�ʧ��" << std::endl;
	}

	// �Ѻ�����ַѹ��ջ�У�Ȼ����ѹ�����()
	lua_getglobal(m_pLuaState, "add");
	lua_pushnumber(m_pLuaState, 3);
	lua_pushnumber(m_pLuaState, 4);

	// ִ��ջ�к�����ַ ����������һ������ֵ��һ������ֵ
	err_rlt = lua_pcall(m_pLuaState, 2, 1, -1);
	if (err_rlt == -1)
	{
		std::cout << "ִ��lua�еĺ���ʧ��" << std::endl;
	}

	// �ٴ�ջ�л�÷���ֵ
	err_rlt = (int)lua_tointeger(m_pLuaState, -1);
	std::cout << "add: " << err_rlt << std::endl;
}

// ���table�е�ֵ
char* LuaParser::GetFeild(lua_State* L, const char* key)
{
	char* rlt = nullptr;

	// keyѹ��ջ�У���Ϊ�������table��ֵ
	lua_pushstring(L, key);
	// ����keyֵ��ȡtable�е�ֵ����key�γɼ�ֵ��
	lua_gettable(L, -2);

	// ջ��value��key��table��ַ
	// �ж�value�Ƿ�Ϊstring
	if (lua_isstring(L, -1))
	{
		// ���valueֵ
		rlt = (char*)lua_tostring(L, -1);

		// ջ��key��table��ַ
		// ����key��������һ��ջ������Ϊtable��ַ
		lua_pop(L, 1);

		return rlt;
	}

	return "wrong";
}

// ����lua�еı�
void LuaParser::ReadTable()
{
	// ����lua�ű�
	luaL_dofile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "��ȡlua�ļ�����" << std::endl;
	}

	// �����Ƶĵ�ַѹ��ջ�������ж��Ƿ�Ϊ��
	lua_getglobal(m_pLuaState, "application");
	if (lua_istable(m_pLuaState, -1))
	{
		// ���Ϊtrue������table
		std::cout << "application is table" << std::endl;

		// ���table�е�ֵ
		char* width = GetFeild(m_pLuaState, "width");
		char* height = GetFeild(m_pLuaState, "height");

		// ���table�е�ֵ
		std::cout << "width is: " << width << std::endl;
		std::cout << "height is: " << height << std::endl;
	}
}

// ���table����һ��ֵ
int LuaParser::TableNext(lua_State* L, int i, char** k, char** v)
{
	// �ж��Ƿ�����һ��ֵ
	if (lua_next(L, i) != 0)
	{
		// ջ��value��key
		// ԭ��ÿȡ��һkey��ֵ���͵���һ��tableֵ��ʹ�±���Զָ��ջ��
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

// ����table
void LuaParser::Foreach()
{
	lua_State* L;
	int idx;
	char* k = nullptr;
	char* v = nullptr;

	// ����ű��ļ�
	luaL_loadfile(m_pLuaState, "output.lua");
	int err_rlt = lua_pcall(m_pLuaState, 0, 0, -1);
	if (err_rlt == -1)
	{
		std::cout << "��ȡlua�ļ�����" << std::endl;
	}

	// ��table��ַѹ��ջ��
	lua_getglobal(m_pLuaState, "application");
	idx = lua_gettop(m_pLuaState);									// ���ָ��ջ�����±�
	lua_pushnil(m_pLuaState);										// ѹ��һ��nil

																	// ����
	while (TableNext(m_pLuaState, 1, &k, &v) != 0)
	{
		std::cout << "key: " << k << ", value: " << v << std::endl;
	}

	lua_close(m_pLuaState);
}