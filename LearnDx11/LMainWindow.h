#pragma once
class LMainWindow
{
public:
	LMainWindow(HINSTANCE hIns);
	~LMainWindow();

	void setup();

public:
	HWND handle() { return m_hwnd; }

protected:
	bool registerClass();
	bool createWindow();

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	SIZE m_size;
};

