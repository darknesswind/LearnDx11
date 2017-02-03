#pragma once
#include "Inc/Keyboard.h"
#include "Inc/Mouse.h"
// #include "Inc/GamePad.h"

class LInput
{
public:
	typedef DirectX::Keyboard KeyBoard;
	typedef DirectX::Keyboard::KeyboardStateTracker KeyBoardTracker;
	typedef DirectX::Mouse Mouse;
	typedef DirectX::Mouse::ButtonStateTracker MouseTracker;
// 	typedef DirectX::GamePad GamePad;
//  	typedef DirectX::GamePad::ButtonStateTracker GamePadTracker;

public:
	LInput();
	~LInput();

	void update();
	bool keyDown(DirectX::Keyboard::Keys key) const { return m_keyTracker.lastState.IsKeyDown(key); }
	bool keyUp(DirectX::Keyboard::Keys key) const { return m_keyTracker.lastState.IsKeyUp(key); }
	bool keyPressed(DirectX::Keyboard::Keys key) const { return m_keyTracker.IsKeyPressed(key); }
	bool keyReleased(DirectX::Keyboard::Keys key) const { return m_keyTracker.IsKeyReleased(key); }

	const KeyBoardTracker& keyboard() const { return m_keyTracker; }
	const MouseTracker& mouse() const { return m_mouseTracker; }

private:
	KeyBoard m_keyboard;
	KeyBoardTracker m_keyTracker;
	Mouse m_mouse;
	MouseTracker m_mouseTracker;
// 	GamePad m_pad;
// 	GamePadTracker m_padTracker;
};

