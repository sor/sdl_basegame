#include "examplegame.h"

void SortState::Init()
{
	GameState::Init();

	if( !image )
	{
		image = IMG_LoadTexture( render, BasePath "asset/graphic/ball.png" );
	}

	float iter = 0;
	const float y_center = game.GetWindowSize().y / 2;

	balls.resize( 1000 );
	for( Ball & ball : balls )
	{
		ball = Ball {
			.x = iter * 40.f,
			.y = y_center + sin( iter / 2.f ) * (y_center / 4.0f),
			.z = 0,
			.w = iter,
		};
		iter += 1.0f;
	}
}

void SortState::UnInit()
{}

void SortState::Events( const u32 frame, const u32 totalMSec, const float deltaT )
{
	// could this be the default?
	SDL_PumpEvents();

	Event event;
	while( SDL_PollEvent( &event ) )
	{
		if( game.HandleEvent( event ) )
			continue;

		if( event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F1 && event.key.repeat == 0 )
			isOrdered = !isOrdered;
	}
}

void SortState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
	for( Ball & ball : balls )
	{
		ball.x -= deltaT * 40.f;
		ball.z = 0.5f + 0.5f * sin( ball.w ); // bounce from 0 to 1
		ball.w += deltaT * 2.0f;
	}
}

void SortState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	SDL_RenderClear( render );

	auto orderByZ = []( Ball & lhs, Ball & rhs )
		{
			return lhs.z < rhs.z;
		};

	if( isOrdered )
		std::sort( balls.begin(), balls.end(), orderByZ );

	for( Ball & ball : balls )
	{
		const int size = 48.f + ball.z * 80.f;
		Rect dst_rect { (int)ball.x - (size / 2), (int)ball.y - (size), size, size };
		SDL_RenderCopy( render, image, EntireRect, &dst_rect );
	}
}
