#ifndef SCREEN_H
#define SCREEN_H

// ��Ļ��
class Screen
{
public:
	// ��ÿ�ȡ��߶ȡ���߱�
	static int Width() { return m_Width; }
	static int Height() { return m_Height; }
	static float AspectRatio() { return m_AspectRatio; }

	// ���ÿ�ȡ��߶ȡ���߱�
	static void SetWidth(int width) { m_Width = width; CalculateAspectRatio(); }
	static void SetHeight(int height) { m_Height = height; CalculateAspectRatio(); }
	static void SetWidthHeight(int width, int height) { m_Width = width; m_Height = height; CalculateAspectRatio(); }

private:
	// �����߱�
	static void CalculateAspectRatio() { m_AspectRatio = m_Width * 1.0f / m_Height; }

private:
	static int m_Width;								// ���
	static int m_Height;							// �߶�
	static float m_AspectRatio;						// ��߱�
};

#endif //SCREEN_H
