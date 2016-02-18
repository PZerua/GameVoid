#include "Controller.h"

Controller::Controller()
{

}

Controller::~Controller()
{

}

void Controller::init(BYTE *mem)
{
	memory = mem;
	bool keydown = false;
	bool keyup = false;
	_requestInterrupt = false;
	_joypadState = 0xFF;
}

BYTE Controller::getJoypadState() const
{
	BYTE res = memory[0xFF00];
	// flip all the bits
	res ^= 0xFF;

	// are we interested in the standard buttons?
	if (!testBit(res, 4))
	{
		BYTE topJoypad = _joypadState >> 4;
		topJoypad |= 0xF0; // turn the top 4 bits on
		res &= topJoypad; // show what buttons are pressed
	}
	else if (!testBit(res, 5))//directional buttons
	{
		BYTE bottomJoypad = _joypadState & 0xF;
		bottomJoypad |= 0xF0;
		res &= bottomJoypad;
	}
	return res;
}

void Controller::keyPressed(const int &key)
{
	bool previouslyUnset = false;

	// if setting from 1 to 0 we may have to request an interupt
	if (testBit(_joypadState, key) == false)
		previouslyUnset = true;

	// remember if a keypressed its bit is 0 not 1
	_joypadState = bitReset(_joypadState, key);

	// button pressed
	bool button = true;

	// is this a standard button or a directional button?
	if (key > 3)
		button = true;
	else // directional button pressed
		button = false;

	BYTE keyReq = memory[0xFF00];
	bool requestInterupt = false;

	// only request interupt if the button just pressed is
	// the style of button the game is interested in
	if (button && !testBit(keyReq, 5))
		requestInterupt = true;

	// same as above but for directional button
	else if (!button && !testBit(keyReq, 4))
		requestInterupt = true;

	// request interupt
	if (requestInterupt && !previouslyUnset)
		_requestInterrupt = true;
}

void Controller::checkControls(SDL_Event *event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_a:
			keyPressed(4);
			break;
		case SDLK_s:
			keyPressed(5);
			break;
		case SDLK_RETURN:
			keyPressed(7);
			break;
		case SDLK_SPACE:
			keyPressed(6);
			break;
		case SDLK_RIGHT:
			keyPressed(0);
			break;
		case SDLK_LEFT:
			keyPressed(1);
			break;
		case SDLK_UP:
			keyPressed(2);
			break;
		case SDLK_DOWN:
			keyPressed(3);
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event->key.keysym.sym)
		{
		case SDLK_a:
			_joypadState = bitSet(_joypadState, 4);
			break;
		case SDLK_s:
			_joypadState = bitSet(_joypadState, 5);
			break;
		case SDLK_RETURN:
			_joypadState = bitSet(_joypadState, 7);
			break;
		case SDLK_SPACE:
			_joypadState = bitSet(_joypadState, 6);
			break;
		case SDLK_RIGHT:
			_joypadState = bitSet(_joypadState, 0);
			break;
		case SDLK_LEFT:
			_joypadState = bitSet(_joypadState, 1);
			break;
		case SDLK_UP:
			_joypadState = bitSet(_joypadState, 2);
			break;
		case SDLK_DOWN:
			_joypadState = bitSet(_joypadState, 3);
			break;
		}
		break;
	}
}

bool Controller::requestInterrupt()
{
	return _requestInterrupt;
}

void Controller::setRequestInterrupt(const bool &state)
{
	_requestInterrupt = state;
}