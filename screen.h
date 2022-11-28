#ifndef SCREEN_H
#define SCREEN_H

// 屏幕类
class Screen
{
public:
	// 获得宽度、高度、宽高比
	static int Width() { return m_Width; }
	static int Height() { return m_Height; }
	static float AspectRatio() { return m_AspectRatio; }

	// 设置宽度、高度、宽高比
	static void SetWidth(int width) { m_Width = width; CalculateAspectRatio(); }
	static void SetHeight(int height) { m_Height = height; CalculateAspectRatio(); }
	static void SetWidthHeight(int width, int height) { m_Width = width; m_Height = height; CalculateAspectRatio(); }

private:
	// 计算宽高比
	static void CalculateAspectRatio() { m_AspectRatio = m_Width * 1.0f / m_Height; }

private:
	static int m_Width;								// 宽度
	static int m_Height;							// 高度
	static float m_AspectRatio;						// 宽高比
};

#endif //SCREEN_H
