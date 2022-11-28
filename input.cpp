#include "input.h"
#include <iostream>
#include "keycode.h"

std::unordered_map<unsigned short, unsigned short> Input::m_KeyEventMap;
Vec2 Input::m_mPosition = { 0, 0 };
short Input::m_mScroll = 0;

// 判断当前帧，键盘按键是否按下
bool Input::GetKey(unsigned short keyCode)
{
	return m_KeyEventMap.count(keyCode) > 0;
}

// 判断当前帧，键盘按键是否处于按下状态
bool Input::GetKeyDown(unsigned short keyCode)
{
	if (m_KeyEventMap.count(keyCode) == 0)
	{
		return false;
	}

	return m_KeyEventMap[keyCode] != KEY_ACTION_UP;
}

// 判断当前帧，键盘按键是否按下并松开
bool Input::GetKeyUp(unsigned short keyCode)
{
	if (m_KeyEventMap.count(keyCode) == 0)
	{
		return false;
	}

	return m_KeyEventMap[keyCode] == KEY_ACTION_UP;
}

// 判断是否按了鼠标某个按钮
bool Input::GetMouseButton(unsigned short mouseButtonIndex)
{
	return GetKey(mouseButtonIndex);
}

// 指定鼠标按键是否处于按下的状态
bool Input::GetMouseButtonDown(unsigned short mouseButtonIndex)
{
	return GetKeyDown(mouseButtonIndex);
}

// 鼠标按钮是否松开
bool Input::GetMouseButtonUp(unsigned short mouseButtonIndex)
{
	return GetKeyUp(mouseButtonIndex);
}

// 记录鼠标滚轴事件
void Input::RecordKey(unsigned short keyCode, unsigned short keyAction)
{
	m_KeyEventMap[keyCode] = keyAction;
}

// 更新
void Input::Update()
{
	for (auto iter = m_KeyEventMap.begin(); iter != m_KeyEventMap.end();)
	{
		if (iter->second == KEY_ACTION_UP)
		{
			iter = m_KeyEventMap.erase(iter);		// 删除元素，返回值指向已删除元素的下一个位置
		}
		else
		{
			++iter;									// 指向下一个位置
		}
	}

	m_mScroll = 0;
}
