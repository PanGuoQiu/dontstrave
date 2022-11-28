#ifndef INPUT_H
#define INPUT_H

#include "keycode.h"
#include <unordered_map>

struct Vec2
{
	unsigned short m_x;
	unsigned short m_y;
};

// ������
class Input
{
public:
	// ��¼�����¼������̰��¼�¼��+1�����̵����¼��-1������¼��Ϊ0��˵����ʱû�а���
	// 0�����֡�1�����¡�2����������
	static void RecordKey(unsigned short keyCode, unsigned short keyAction);

	// �жϵ�ǰ֡�����̰����Ƿ���
	static bool GetKey(unsigned short keyCode);

	// �жϵ�ǰ֡�����̰����Ƿ��ڰ���״̬
	static bool GetKeyDown(unsigned short keyCode);

	// �жϵ�ǰ֡�����̰����Ƿ��²��ɿ�
	static bool GetKeyUp(unsigned short keyCode);

	// ˢ֡
	static void Update();

	// �ж��Ƿ������ĳ����ť
	// 0�������1���Ҽ���2���м��
	static bool GetMouseButton(unsigned short mouseButtonIndex);

	// ָ����갴���Ƿ��ڰ��µ�״̬
	static bool GetMouseButtonDown(unsigned short mouseButtonIndex);

	// ��갴ť�Ƿ��ɿ�
	static bool GetMouseButtonUp(unsigned short mouseButtonIndex);

	// ������λ��
	static Vec2 MousePosition() { return m_mPosition; }
	// �������λ��
	static void SetMousePosition(unsigned short x, unsigned short y)
	{
		m_mPosition.m_x = x;
		m_mPosition.m_y = y;
	}

	// ���������ֵ
	static short MouseScroll() { return m_mScroll; }
	// ��¼�������¼�
	static void RecordScroll(short mouseScroll) { m_mScroll += mouseScroll; }

private:
	// ����״̬���������¡�ż������0��ʾû�м�¼
	static std::unordered_map<unsigned short, unsigned short> m_KeyEventMap;
	static Vec2 m_mPosition;						// ���λ��
	static short m_mScroll;							// ������ֵ
};

#endif //INPUT_H