#include "Window.h"

SDL_Renderer *Window::mRenderer = NULL;

Window::Window()
{
	//Initialize non-existant window
	mWindow = NULL;

	_pressing = false;
	_isFullscreen = false;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mShown = false;
	mClosed = false;
	mWindowID = -1;

	mWidth = 0;
	mHeight = 0;
}

bool Window::init(string screenTitle, int xPos, int yPos)
{
	//Create window
	mWindow = SDL_CreateWindow( screenTitle.c_str(), xPos, yPos, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	cout << "[GAME] Window created" << endl;
	if( mWindow != NULL )
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;

		//Create renderer for window
		mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		cout << "[GAME] Renderer created: Accelerated, VSYNC" << endl;

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		SDL_RenderSetLogicalSize(mRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

		if( mRenderer == NULL )
		{
			printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			SDL_DestroyWindow( mWindow );
			mWindow = NULL;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor( mRenderer, 0, 0, 0, 255 );

			//Grab window identifier
			mWindowID = SDL_GetWindowID( mWindow );

			//Flag as opened
			mShown = true;
		}
	}
	else
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}

	return mWindow != NULL && mRenderer != NULL;
}

void Window::handleEvent( SDL_Event& e )
{
	//If an event was detected for this window
	if( e.type == (unsigned) SDL_WINDOWEVENT)
	{
		switch( e.window.event )
		{
			//Window appeared
			case SDL_WINDOWEVENT_SHOWN:
				mShown = true;
				break;

			//Window disappeared
			case SDL_WINDOWEVENT_HIDDEN:
				mShown = false;
				break;

			//Get new dimensions and repaint
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				SDL_RenderPresent( mRenderer );
				break;

			//Repaint on expose
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent( mRenderer );
				break;

			//Mouse enter
			case SDL_WINDOWEVENT_ENTER:
				mMouseFocus = true;
				break;

			//Mouse exit
			case SDL_WINDOWEVENT_LEAVE:
				mMouseFocus = false;
				break;

			//Keyboard focus gained
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				mKeyboardFocus = true;
				break;

			//Keyboard focus lost
			case SDL_WINDOWEVENT_FOCUS_LOST:
				mKeyboardFocus = false;
				break;

			//Window minimized
			case SDL_WINDOWEVENT_MINIMIZED:
				mMinimized = true;
				break;

			//Window maxized
			case SDL_WINDOWEVENT_MAXIMIZED:
				mMinimized = false;
				break;

			//Window restored
			case SDL_WINDOWEVENT_RESTORED:
				mMinimized = false;
				break;

			//Hide on close
			case SDL_WINDOWEVENT_CLOSE:
				free();
				mClosed = true;
				break;
		}
	}
	switch (e.type)
	{
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
				case SDLK_F11:
					if (!_isFullscreen && !_pressing)
					{
						SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
						_isFullscreen = true;
						_pressing = true;
					}
					else if (_isFullscreen && !_pressing)
					{
						SDL_SetWindowFullscreen(mWindow, 0);
						_isFullscreen = false;
						_pressing = true;
					}
					break;
			}
			break;
		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
				case SDLK_F11:
					_pressing = false;
					break;
			}
			break;
	}
}

void Window::focus()
{
	//Restore window if needed
	if( !mShown )
	{
		SDL_ShowWindow( mWindow );
	}

	//Move window forward
	SDL_RaiseWindow( mWindow );
}

void Window::Present()
{
	if( !mMinimized )
	{
		//Update screen
		SDL_RenderPresent( mRenderer );
	}
}

void Window::Clear()
{
	if ( !mMinimized )
	{
	    //Clear screen
	    SDL_RenderClear( mRenderer );
	}
}

void Window::free()
{
	if( mWindow != NULL )
	{
		SDL_DestroyWindow( mWindow );
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}

bool Window::hasMouseFocus()
{
	return mMouseFocus;
}

bool Window::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool Window::isMinimized()
{
	return mMinimized;
}

bool Window::isShown()
{
	return mShown;
}

bool Window::isClosed()
{
	return mClosed;
}

SDL_Renderer *Window::getRender()
{
	return mRenderer;
}

void Window::setFocus(bool focus)
{
	mMouseFocus = focus;
}

bool Window::isFullscreen()
{
	return _isFullscreen;
}
