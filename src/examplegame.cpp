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
	if( !font )
	{
		font = TTF_OpenFont( BasePath "asset/font/RobotoSlab-Bold.ttf", 24 );
		TTF_SetFontHinting( font, TTF_HINTING_LIGHT );
	}

	if( !image )
	{
		image = IMG_LoadTexture( render, BasePath "asset/graphic/background.png" );
	}
}

void ExampleState::UnInit()
{
	/*
	TTF_CloseFont( font );
	SDL_DestroyTexture( image );
	SDL_DestroyTexture( blendedText );
	font = nullptr;
	image = nullptr;
	blendedText = nullptr;
	*/
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

			if( blendedText != nullptr )
				SDL_DestroyTexture( blendedText );

			Surface * surf = TTF_RenderUTF8_Blended_Wrapped( font, text, white, 1250 );
			blendedText = SDL_CreateTextureFromSurface( render, surf );
			SDL_FreeSurface( surf );

			u32 fmt;
			int access;
			SDL_QueryTexture( blendedText, &fmt, &access, &blendedTextSize.x, &blendedTextSize.y );
		}

		// Draw the text on top
		{
			constexpr const Point p { 32, 50 };
			SDL_SetTextureColorMod( blendedText, 0, 0, 0 );
			for( const Point & pd : shadowOffsets )
			{
				const Rect dst_rect = Rect{ p.x + pd.x, p.y + pd.y, blendedTextSize.x, blendedTextSize.y };
				SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
			}

			SDL_SetTextureColorMod( blendedText, 255, 255, 255 );
			const Rect dst_rect = { p.x, p.y, blendedTextSize.x, blendedTextSize.y };
			SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
		}
	}
}

void ExampleState2::Init()
{
	font = TTF_OpenFont( BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 24 );
	TTF_SetFontHinting( font, TTF_HINTING_NONE );

	plasmaSrf = SDL_CreateRGBSurfaceWithFormat( 0, 1280 / Scale, 960 / Scale, 32, SDL_PIXELFORMAT_RGBA32 );

	// Set to smoothed rendering for the plasma texture
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "best" );

	plasmaTex = SDL_CreateTexture( render, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 1280 / Scale, 960 / Scale );

	// Reset to "pixelated" for further textures
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" );
}

void ExampleState2::UnInit()
{
	SDL_FreeSurface( plasmaSrf );
	SDL_DestroyTexture( plasmaTex );
	plasmaSrf = nullptr;
	plasmaTex = nullptr;

	ExampleState::UnInit();
}

void ExampleState2::Events( const u32 frame, const u32 totalMSec, const float deltaT )
{
	SDL_PumpEvents();

	Event event;
	while( SDL_PollEvent( &event ) )
	{
		if( game.HandleEvent( event ) )
			continue;
	}

	const u8 * key_array = SDL_GetKeyboardState( nullptr );
	if( key_array[SDL_SCANCODE_DOWN] )
	{
		brightness -= 1;
	}
	if( key_array[SDL_SCANCODE_UP] )
	{
		brightness += 1;
	}
}

static inline float plasma(float x, float y, float time)
{
	const float cx = x * 0.1f + 0.5f * sin( time / 5.0f );
	const float cy = y * 0.1f + 0.5f * cos( time / 3.0f );
	float v = 0.0f;
	v += sin( (x + time) );
	v += sin( (y + time) / 2.0f );
	v += sin( (x + y + time) / 2.0f );
	v += sin( sqrt( (cx * cx + cy * cy) * 100.0f + 1.0f ) + time );
	v = v / 2.0f;
	return v;
}

void ExampleState2::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
	u8 *        px       = (u8 *)plasmaSrf->pixels;
	const int   pitch    = plasmaSrf->pitch;
	const int   h        = plasmaSrf->h;
	const int   w        = plasmaSrf->w;
	const float xy_scale = Scale * 0.015f;
	const float xy_frame = (float)frame * 0.05f;
#pragma omp parallel
#pragma omp for
	for( int y = 0; y < h; ++y )
	{
		for( int x = 0; x < w; ++x )
		{
			const float v = plasma( (x - w / 2) * xy_scale, (y - h / 2) * xy_scale, xy_frame );
			const int offset = x * 4 + y * pitch;
			px[offset + 0] = std::max<int>( 0, std::min<int>( 255, 4 * brightness * (.5f + .5f * sin( M_PI * v )) + brightness - 64 ) );
			px[offset + 1] = std::max<int>( 0, std::min<int>( 255, 4 * brightness * (.5f + .5f * cos( M_PI * v )) + brightness - 64 ) );
			px[offset + 2] = std::max<int>( 0, std::min<int>( 255, 4 * brightness - 255 ) );
			px[offset + 3] = 255;
		}
	}
}

void ExampleState2::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	// No clear needed since everything is overdrawn

	// Draw the plasma
	{
		SDL_UpdateTexture( plasmaTex, EntireRect, plasmaSrf->pixels, plasmaSrf->pitch );
		const Rect dst_rect { 0, 0, plasmaSrf->w * Scale, plasmaSrf->h * Scale };
		SDL_RenderCopy( render, plasmaTex, EntireRect, &dst_rect );
	}

	// Prepare the text as Texture
	if( blendedText == nullptr )
	{
		constexpr const char * text =
			"Use down and up arrow keys\n"
			" to change the brightness\n"
			"  of the plasma effect!";

		if( blendedText != nullptr )
			SDL_DestroyTexture( blendedText );

		Surface * surf = TTF_RenderUTF8_Blended_Wrapped( font, text, white, 1250 );
		blendedText = SDL_CreateTextureFromSurface( render, surf );
		SDL_FreeSurface( surf );

		u32 fmt;
		int access;
		SDL_QueryTexture( blendedText, &fmt, &access, &blendedTextSize.x, &blendedTextSize.y );
	}

	// Draw the text on top
	{
		const Point p {
			game.GetWindowSize().x / 3,
			game.GetWindowSize().y - 150
		};

		SDL_SetTextureColorMod( blendedText, 0, 0, 0 );
		for( const Point & pd : shadowOffsets )
		{
			const Rect dst_rect = p + (pd * 2) + Rect{ 0, 0, blendedTextSize.x, blendedTextSize.y };
			SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
		}

		SDL_SetTextureColorMod( blendedText, 255, 255, 255 );
		const Rect dst_rect = p + Rect{ 0, 0, blendedTextSize.x, blendedTextSize.y };
		SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
	}
}
