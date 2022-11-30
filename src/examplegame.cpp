#include "examplegame.h"

ExampleGame::ExampleGame()
	: Game( "Example SDL Game", Point{ 1280, 960 }, false )
{
	// Will be freed in Game dtor
	allStates = {
		new IntroState( *this, render ),    // 0
		new PlasmaState( *this, render ),   // 1
		new SortState( *this, render ),     // 2
		new CameraState( *this, render ),   // 3
	};

	// The virtual call is ok here
	SetNextState( 0 );

	SetPerfDrawMode(PerformanceDrawMode::Title);
}

bool ExampleGame::HandleEvent( const Event event )
{
	// Global Keybindings, will be checked before the ones in GameState
	switch( event.type )
	{
		case SDL_KEYDOWN:
		{
			const Keysym & what_key = event.key.keysym;

			if( what_key.scancode == SDL_SCANCODE_1 )
			{
				// Both Game and GameState can change the State,
				// but it will only be changed after the current frame
				SetNextState( 0 );
				return true;
			}
			else if( what_key.scancode == SDL_SCANCODE_2 )
			{
				SetNextState( 1 );
				return true;
			}
			else if( what_key.scancode == SDL_SCANCODE_3 )
			{
				SetNextState( 2 );
				return true;
			}
			else if( what_key.scancode == SDL_SCANCODE_4 )
			{
				SetNextState( 3 );
				return true;
			}
			break;
		}

		default:
			break;
	}
	return Game::HandleEvent( event );
}
