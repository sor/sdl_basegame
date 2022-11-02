#include "examplegame.h"

ExampleGame::ExampleGame()
	: Game( "Example SDL Game", Point{ 1280, 960 }, false )
{
	// Will be freed in Game dtor
	allStates = {
		new ExampleState( *this, render ),  // 0
		new ExampleState2( *this, render ), // 1
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
			break;
		}

		default:
			break;
	}
	return Game::HandleEvent( event );
}

void ExampleState::Init()
{
	font = TTF_OpenFont( "../asset/font/RobotoSlab-Bold.ttf", 24 );
	TTF_SetFontHinting( font, TTF_HINTING_LIGHT );
	image = IMG_LoadTexture( render, "../asset/graphic/background.png" );
}

void ExampleState::UnInit()
{
	TTF_CloseFont( font );
	SDL_DestroyTexture( image );
	SDL_DestroyTexture( blendedText );
	font = nullptr;
	image = nullptr;
	blendedText = nullptr;
}

void ExampleState::Events( const u32 frame, const u32 totalMSec, const float deltaT )
{
	SDL_PumpEvents();

	Event event;
	while( SDL_PollEvent( &event ) )
	{
		if( game.HandleEvent( event ) )
			continue;

		switch( event.type )
		{
			case SDL_KEYDOWN:
			{
				const Keysym & what_key = event.key.keysym;

				if( what_key.scancode == SDL_SCANCODE_F9 )
				{
					// crash/shutdown, since State #2 does not exist
					game.SetNextState( 2 );
				}
				else if( what_key.scancode == SDL_SCANCODE_ESCAPE )
				{
					game.SetNextState( 0 );
				}

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
				game.SetNextState( 1 );
				break;

			default:
				break;
		}
	}
}

void ExampleState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
}

void ExampleState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	{
		const Rect dst_rect { 0, 0, 1280, 960 };
		SDL_RenderCopy( render, image, EntireRect, &dst_rect );
	}

	// Poor persons benchmark
	//for (uint x = 0; x < 100; ++x)
	{
		// uncomment to use the cache, comment out to disable
		if( blendedText == nullptr )
		{
			constexpr const char * text =
				"                                          --== Super Mega Gamey Game 3000 ==--\n\n"
				"Dies ist ein Typoblindtext. An ihm kann man sehen, ob alle Buchstaben da sind und wie sie aussehen. Manchmal benutzt man Worte wie Hamburgefonts, Rafgenduks oder Handgloves, um Schriften zu testen. Manchmal Sätze, die alle Buchstaben des Alphabets enthalten - man nennt diese Sätze »Pangrams«. Sehr bekannt ist dieser: The quick brown fox jumps over the lazy old dog. Oft werden in Typoblindtexte auch fremdsprachige Satzteile eingebaut (AVAIL® and Wefox™ are testing aussi la Kerning), um die Wirkung in anderen Sprachen zu testen. In Lateinisch sieht zum Beispiel fast jede Schrift gut aus. Quod erat demonstrandum. Seit 1975 fehlen in den meisten Testtexten die Zahlen, weswegen nach TypoGb. 204 § ab dem Jahr 2034 Zahlen in 86 der Texte zur Pflicht werden. Nichteinhaltung wird mit bis zu 245 € oder 368 $ bestraft. Genauso wichtig in sind mittlerweile auch Âçcèñtë, die in neueren Schriften aber fast immer enthalten sind. Ein wichtiges aber schwierig zu integrierendes Feld sind OpenType-Funktionalitäten. Je nach Software und Voreinstellungen können eingebaute Kapitälchen, Kerning oder Ligaturen (sehr pfiffig) nicht richtig dargestellt werden."
				"\n\nPress any key to continue!";

			Surface * surf = TTF_RenderUTF8_Blended_Wrapped( font, text, white, 1250 );

			if( blendedText != nullptr )
				SDL_DestroyTexture( blendedText );

			blendedText = SDL_CreateTextureFromSurface( render, surf );

			u32 fmt;
			int access;
			SDL_QueryTexture( blendedText, &fmt, &access, &blendedTextSize.x, &blendedTextSize.y );

			SDL_FreeSurface( surf );
		}

		SDL_SetTextureColorMod( blendedText, 0, 0, 0 );
		constexpr const Point p { 32, 50 };
		for( const Point & pd : shadowOffsets )
		{
			const Rect dst_rect_new_method = p + pd + Rect{ 0, 0, blendedTextSize.x, blendedTextSize.y };
			const Rect dst_rect = Rect{ p.x + pd.x, p.y + pd.y, blendedTextSize.x, blendedTextSize.y };
			SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
		}

		{
			SDL_SetTextureColorMod( blendedText, 255, 255, 255 );
			const Rect dst_rect = { p.x, p.y, blendedTextSize.x, blendedTextSize.y };
			SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
		}
	}

	SDL_RenderPresent( render );
}

void ExampleState2::Init()
{
	font = TTF_OpenFont( "../asset/font/MonkeyIsland-1991-refined.ttf", 24 );
	TTF_SetFontHinting( font, TTF_HINTING_NONE );
	image = IMG_LoadTexture( render, "../asset/graphic/background.png" );
	SDL_SetTextureColorMod( image, 127, 255, 255 );
}
