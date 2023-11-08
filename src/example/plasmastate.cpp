#include "examplegame.h"

void PlasmaState::Init()
{
	font = TTF_OpenFont( BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 24 );
	TTF_SetFontHinting( font, TTF_HINTING_NONE );

	const Point & winSize = game.GetWindowSize();
	const Point resolution = winSize / Scale;
	plasmaSrf = SDL_CreateRGBSurfaceWithFormat( 0, resolution.x, resolution.y, 32, SDL_PIXELFORMAT_RGBA32 );

	// Set to smoothed rendering for the plasma texture
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "best" );
	plasmaTex = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, resolution.x, resolution.y );
	// Reset to "pixelated" for further textures
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" );
}

void PlasmaState::UnInit()
{
	SDL_FreeSurface( plasmaSrf );
	SDL_DestroyTexture( plasmaTex );
	plasmaSrf = nullptr;
	plasmaTex = nullptr;

	IntroState::UnInit();
}

bool PlasmaState::HandleEvent( const Event & event )
{
	switch( event.type )
	{
		case SDL_MOUSEWHEEL:
			brightness += event.wheel.y*3;
			return true;
	}

	return false;
}

void PlasmaState::Input()
{
	// Is not supressed during ImGui input
	const u8 * key_array = SDL_GetKeyboardState( nullptr );
	if( key_array[SDL_SCANCODE_DOWN] )
	{
		brightness -= 1;
	}
	if( key_array[SDL_SCANCODE_UP] ) // Not an else-if, as both buttons can be held at the same time
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

void PlasmaState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
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

void PlasmaState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	// Draw the plasma
	{
		SDL_UpdateTexture( plasmaTex, EntireRect, plasmaSrf->pixels, plasmaSrf->pitch );
		const Rect dst_rect { 0, 0, plasmaSrf->w * Scale, plasmaSrf->h * Scale };
		SDL_RenderCopy( renderer, plasmaTex, EntireRect, &dst_rect );
	}

	const Point & winSize = GetGame().GetWindowSize();

	// Prepare the text as Texture
	if( blendedText == nullptr )
	{
		constexpr const char * text =
			"Use mouse-wheel or [DOWN] and [UP] arrow keys\n"
			"                to change the brightness\n"
			"                  of the plasma effect!";

		if( blendedText != nullptr )
			SDL_DestroyTexture( blendedText );

		Surface * surf = TTF_RenderUTF8_Blended_Wrapped( font, text, white, winSize.x - 30 );
		blendedText = SDL_CreateTextureFromSurface( renderer, surf );
		SDL_FreeSurface( surf );

		u32 fmt;
		int access;
		SDL_QueryTexture( blendedText, &fmt, &access, &blendedTextSize.x, &blendedTextSize.y );
	}

	// Draw the text on top
	{
		const Point p {
			winSize.x / 5,
			winSize.y - 150
		};

		SDL_SetTextureColorMod( blendedText, 0, 0, 0 );
		for( const Point & pd : shadowOffsets )
		{
			const Rect dst_rect = p + (pd * 2) + Rect{ 0, 0, blendedTextSize.x, blendedTextSize.y };
			SDL_RenderCopy( renderer, blendedText, EntireRect, &dst_rect );
		}

		SDL_SetTextureColorMod( blendedText, 255, 255, 255 );
		const Rect dst_rect = p + Rect{ 0, 0, blendedTextSize.x, blendedTextSize.y };
		SDL_RenderCopy( renderer, blendedText, EntireRect, &dst_rect );
	}
}
