#ifndef INPUT_H
#define INPUT_H

#include "keycode.h"
#include <unordered_map>

struct Vec2
{
	unsigned short m_x;
	unsigned short m_y;
};

// 输入类
class Input
{
public:
	// 记录按键事件，键盘按下记录数+1，键盘弹起记录数-1，当记录数为0，说明此时没有按键
	// 0：松手、1：按下、2：持续按下
	static void RecordKey(unsigned short keyCode, unsigned short keyAction);

	// 判断当前帧，键盘按键是否按下
	static bool GetKey(unsigned short keyCode);

	// 判断当前帧，键盘按键是否处于按下状态
	static bool GetKeyDown(unsigned short keyCode);

	// 判断当前帧，键盘按键是否按下并松开
	static bool GetKeyUp(unsigned short keyCode);

	// 刷帧
	static void Update();

	// 判断是否按了鼠标某个按钮
	// 0：左键、1：右键、2：中间键
	static bool GetMouseButton(unsigned short mouseButtonIndex);

	// 指定鼠标按键是否处于按下的状态
	static bool GetMouseButtonDown(unsigned short mouseButtonIndex);

	// 鼠标按钮是否松开
	static bool GetMouseButtonUp(unsigned short mouseButtonIndex);

	// 获得鼠标位置
	static Vec2 MousePosition() { return m_mPosition; }
	// 设置鼠标位置
	static void SetMousePosition(unsigned short x, unsigned short y)
	{
		m_mPosition.m_x = x;
		m_mPosition.m_y = y;
	}

	// 获得鼠标滚轴值
	static short MouseScroll() { return m_mScroll; }
	// 记录鼠标滚轴事件
	static void RecordScroll(short mouseScroll) { m_mScroll += mouseScroll; }

private:
	// 按键状态，奇数按下、偶数弹起、0表示没有记录
	static std::unordered_map<unsigned short, unsigned short> m_KeyEventMap;
	static Vec2 m_mPosition;						// 鼠标位置
	static short m_mScroll;							// 鼠标滚轴值
};

#endif //INPUT_H