#pragma once
class LD3DApplication;
class LMainWindow
{
public:
	LMainWindow(LD3DApplication* pApp);
	~LMainWindow();

	void setup();
	bool processMessage();
	void resize(ushort width, ushort height);

public:
	HWND handle() const { return m_hwnd; }
	long width() const { return m_size.cx; }
	long height() const { return m_size.cy; }
	float aspectRadio() const { return static_cast<float>(m_size.cx) / m_size.cy; }

protected:
	bool registerClass();
	bool createWindow();
	LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam);
	void onResize();

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static bool s_bQuitFlag;

private:
	LD3DApplication* m_pApp;
	HWND m_hwnd;
	SIZE m_size;
};

