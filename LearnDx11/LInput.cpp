#include "stdafx.h"
#include "LInput.h"


LInput::LInput()
{
}


LInput::~LInput()
{
}

void LInput::update()
{
	m_keyTracker.Update(m_keyboard.GetState());
	m_mouseTracker.Update(m_mouse.GetState());
//  	m_padTracker.Update(m_pad.GetState(0));
}
