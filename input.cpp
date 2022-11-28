#include "input.h"
#include <iostream>
#include "keycode.h"

std::unordered_map<unsigned short, unsigned short> Input::m_KeyEventMap;
Vec2 Input::m_mPosition = { 0, 0 };
short Input::m_mScroll = 0;

// �жϵ�ǰ֡�����̰����Ƿ���
bool Input::GetKey(unsigned short keyCode)
{
	return m_KeyEventMap.count(keyCode) > 0;
}

// �жϵ�ǰ֡�����̰����Ƿ��ڰ���״̬
bool Input::GetKeyDown(unsigned short keyCode)
{
	if (m_KeyEventMap.count(keyCode) == 0)
	{
		return false;
	}

	return m_KeyEventMap[keyCode] != KEY_ACTION_UP;
}

// �жϵ�ǰ֡�����̰����Ƿ��²��ɿ�
bool Input::GetKeyUp(unsigned short keyCode)
{
	if (m_KeyEventMap.count(keyCode) == 0)
	{
		return false;
	}

	return m_KeyEventMap[keyCode] == KEY_ACTION_UP;
}

// �ж��Ƿ������ĳ����ť
bool Input::GetMouseButton(unsigned short mouseButtonIndex)
{
	return GetKey(mouseButtonIndex);
}

// ָ����갴���Ƿ��ڰ��µ�״̬
bool Input::GetMouseButtonDown(unsigned short mouseButtonIndex)
{
	return GetKeyDown(mouseButtonIndex);
}

// ��갴ť�Ƿ��ɿ�
bool Input::GetMouseButtonUp(unsigned short mouseButtonIndex)
{
	return GetKeyUp(mouseButtonIndex);
}

// ��¼�������¼�
void Input::RecordKey(unsigned short keyCode, unsigned short keyAction)
{
	m_KeyEventMap[keyCode] = keyAction;
}

// ����
void Input::Update()
{
	for (auto iter = m_KeyEventMap.begin(); iter != m_KeyEventMap.end();)
	{
		if (iter->second == KEY_ACTION_UP)
		{
			iter = m_KeyEventMap.erase(iter);		// ɾ��Ԫ�أ�����ֵָ����ɾ��Ԫ�ص���һ��λ��
		}
		else
		{
			++iter;									// ָ����һ��λ��
		}
	}

	m_mScroll = 0;
}
