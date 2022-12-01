#include "examplegame.h"

void CameraState::Init()
{
	GameState::Init();

	if( !bg[0] )
	{
		//SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "best" );

		bg[0] = IMG_LoadTexture( render, BasePath "asset/graphic/bg-layer-4.png" );
		bg[1] = IMG_LoadTexture( render, BasePath "asset/graphic/bg-layer-3.png" );
		bg[2] = IMG_LoadTexture( render, BasePath "asset/graphic/bg-layer-2.png" );
		bg[3] = IMG_LoadTexture( render, BasePath "asset/graphic/bg-layer-1.png" );

		SDL_QueryTexture( bg[0], nullptr, nullptr, &bgSize[0].x, &bgSize[0].y );
		SDL_QueryTexture( bg[1], nullptr, nullptr, &bgSize[1].x, &bgSize[1].y );
		SDL_QueryTexture( bg[2], nullptr, nullptr, &bgSize[2].x, &bgSize[2].y );
		SDL_QueryTexture( bg[3], nullptr, nullptr, &bgSize[3].x, &bgSize[3].y );

		SDL_SetTextureColorMod( bg[0], 195, 195, 195 );
		SDL_SetTextureColorMod( bg[1], 195, 195, 195 );
		SDL_SetTextureColorMod( bg[2], 225, 225, 225 );
		SDL_SetTextureColorMod( bg[3], 225, 225, 255 );

		SDL_SetTextureAlphaMod( bg[2], 210 );
		SDL_SetTextureAlphaMod( bg[3], 180 );

		SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" );
	}

	// Reinit on reenter
	cam = { .x = 0, .y = 0 };
}

void CameraState::UnInit()
{
	// TODO
}

void CameraState::Events( const u32 frame, const u32 totalMSec, const float deltaT )
{
	// Could this be the default?
	SDL_PumpEvents();

	Event event;
	while( SDL_PollEvent( &event ) )
	{
		if( game.HandleEvent( event ) )
			continue;

		if( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
		{
			if( event.key.keysym.scancode == SDL_SCANCODE_F1 ) bgIsVisible[0] = !bgIsVisible[0];
			if( event.key.keysym.scancode == SDL_SCANCODE_F2 ) bgIsVisible[1] = !bgIsVisible[1];
			if( event.key.keysym.scancode == SDL_SCANCODE_F3 ) bgIsVisible[2] = !bgIsVisible[2];
			if( event.key.keysym.scancode == SDL_SCANCODE_F4 ) bgIsVisible[3] = !bgIsVisible[3];

			// Toggle all
			if( event.key.keysym.scancode == SDL_SCANCODE_F5 )
				bgIsVisible[0] = bgIsVisible[1] = bgIsVisible[2] = bgIsVisible[3] = !bgIsVisible[0];

			if( event.key.keysym.scancode == SDL_SCANCODE_F6 ) isInverted = !isInverted;
			if( event.key.keysym.scancode == SDL_SCANCODE_F7 ) isEased = !isEased;
			if( event.key.keysym.scancode == SDL_SCANCODE_F8 ) isFlux = !isFlux;
		}
		else if( event.type == SDL_MOUSEBUTTONDOWN
			||   event.type == SDL_MOUSEMOTION && event.motion.state != 0 )
		{
			const Point  halfWinSize  = game.GetWindowSize() / 2;
			const FPoint halfWinSizeF = { (float)halfWinSize.x, (float)halfWinSize.y };
			const FPoint mousePos     = { (float)event.motion.x, (float)event.motion.y };

			mouseOffset = isInverted
				? halfWinSizeF - mousePos
				: mousePos - halfWinSizeF;
		}
		else if( event.type == SDL_MOUSEBUTTONUP )
		{
			mouseOffset = { 0, 0 };
		}
	}

	const u8 * key_array = SDL_GetKeyboardState( nullptr );
	float factor = key_array[SDL_SCANCODE_RSHIFT]
		? 600.f
		: 80.0f;

	if( key_array[SDL_SCANCODE_LEFT]  ) cam.x += deltaT * factor;
	if( key_array[SDL_SCANCODE_RIGHT] )	cam.x -= deltaT * factor;

	if( key_array[SDL_SCANCODE_UP]    ) cam.y += deltaT * factor;
	if( key_array[SDL_SCANCODE_DOWN]  ) cam.y -= deltaT * factor;
}

void CameraState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
	if( isEased )
	{
		SDL_FPoint diff = (mouseOffset - mouseOffsetEased);

		//constexpr const float thresh = 20.0f; // 1.2f;
		//if( -thresh < diff.x && diff.x < thresh ) diff.x = 0;
		//if( -thresh < diff.y && diff.y < thresh ) diff.y = 0;

		//constexpr const float thresh = 2.2f;
		//if( -thresh < diff.x && diff.x < thresh ) { mouseOffsetEased.x = mouseOffset.x; diff.x = 0; }
		//if( -thresh < diff.y && diff.y < thresh ) { mouseOffsetEased.y = mouseOffset.y; diff.y = 0; }

		mouseOffsetEased += diff * max( 0.1f, min( 0.4f, deltaT * 10.0f ) );
	}
	else
	{
		mouseOffsetEased = mouseOffset;
	}
}

void CameraState::Render( const u32 frame, u32 totalMSec, const float deltaT )
{
	SDL_RenderClear( render );

	// Try the limits, moments before wraparound
	//totalMSec += 2147470000u + 2147480000u;

	const Point winSize = game.GetWindowSize();
	const FPoint flux = isFlux
		? FPoint {
			.x = (float)sin( totalMSec / 650.0f ) * 5.0f,
			.y = (float)sin( totalMSec / 500.0f ) * 10.0f
			     + (float)sin( totalMSec / 850.0f ) * 5.0f
			     + (float)cos( totalMSec / 1333.0f ) * 5.0f }
	    : FPoint { 0, 0 };
	const FPoint fluxCam = cam + flux + mouseOffsetEased;

	for( int i = 0; i <= 3; ++i ) // The 4 layers, rendered back to front
	{
		if( !bgIsVisible[i] )
			continue;

		const Point size = bgSize[i];
		const FPoint offset = bgStart[i] + fluxCam * bgFactor[i];
		for( float x = offset.x; x < winSize.x; x += size.x * 2 )
		{
			for( float y = offset.y; y < winSize.y; y += size.y * 2 )
			{
				Rect off = { .x = (int)x, .y = (int)y, .w = size.x * 2, .h = size.y * 2 };
				SDL_RenderCopy( render, bg[i], EntireRect, &off );

				// Makes only sense with texture hint == best
				//FRect offset = { .x = x, .y = y, .w = size.x * 2.0f, .h = size.y * 2.0f };
				//SDL_RenderCopyF( render, bg[i], EntireRect, &offset );
			}
		}
	}
}
