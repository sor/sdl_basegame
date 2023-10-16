#include "examplegame.h"

constexpr bool doBenchmark = true;

ExampleGame::ExampleGame()
	: Game( "Example SDL Game", Point{ 1280, 960 }, !doBenchmark )
{
	// Will be freed in Game dtor
	allStates = {
		new IntroState      ( *this, renderer ),   // 0
		new PlasmaState     ( *this, renderer ),   // 1
		new SortState       ( *this, renderer ),   // 2
		new CameraState     ( *this, renderer ),   // 3
		new ShooterState    ( *this, renderer ),   // 4
		new MapEditorState  ( *this, renderer ),   // 5
	};

	// The virtual call is ok here
	SetNextState( 0 );

	SetPerfDrawMode( PerformanceDrawMode::Title );
}

bool ExampleGame::HandleEvent( const Event & event )
{
	// Global Keybindings, will be checked before the ones in GameState
	if( Game::HandleEvent( event ) )
		return true;

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
			else if( what_key.scancode == SDL_SCANCODE_5 )
			{
				SetNextState( 4 );
				return true;
			}
			else if( what_key.scancode == SDL_SCANCODE_6 )
			{
				SetNextState( 5 );
				return true;
			}
			break;
		}

		default:
			break;
	}

	return false;
}
