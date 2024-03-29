#include <assert.h>
#include "InputMan.h"


InputMan *InputMan::pInputMan = 0;

void InputMan::Create()
{
	InputMan::privCreate();
}

InputMan::InputMan()
{
	this->pKeyboard = new Keyboard();
	assert(this->pKeyboard);
}

void InputMan::privCreate()
{
	InputMan::pInputMan = new InputMan();
	assert(pInputMan);
}

InputMan::~InputMan()
{
	delete this->pKeyboard;
}

Keyboard *InputMan::GetKeyboard()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->pKeyboard;
}

Mouse *InputMan::GetMouse()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->pMouse;
}


InputMan *InputMan::privInstance()
{
	assert(pInputMan);
	return pInputMan;
}

