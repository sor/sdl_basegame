#include "examplegame.h"

void ShooterState::Init()
{
	CameraState::Init();

	if( !projectile[0] )
	{
		projectile[0] = IMG_LoadTexture( render, BasePath "asset/graphic/fire.png"   );
		projectile[1] = IMG_LoadTexture( render, BasePath "asset/graphic/ice.png"    );
		projectile[2] = IMG_LoadTexture( render, BasePath "asset/graphic/poison.png" );

		SDL_SetTextureColorMod( projectile[2], 191, 191, 191 );
	}
}

void ShooterState::UnInit()
{
	// TODO
}

void ShooterState::Events( const u32 frame, const u32 totalMSec, const float deltaT )
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
		else if( event.type == SDL_MOUSEBUTTONDOWN )
		{
			const FPoint mousePos = FPoint {
				(float)event.button.x,
				(float)event.button.y } - mouseOffsetEased - cam;
			//enemyProjectiles.push_back( mousePos );
			//rvProjectiles.Spawn(mousePos);
			//rvProjectiles.Spawn({1,1});
			SpawnProjectile( mousePos );
		}
		else if( event.type == SDL_MOUSEMOTION )
		{
			const Point  halfWinSize  = game.GetWindowSize() / 2;
			const FPoint halfWinSizeF = { (float)halfWinSize.x, (float)halfWinSize.y };
			const FPoint mousePos     = { (float)event.motion.x, (float)event.motion.y };

			mouseOffset = halfWinSizeF - mousePos;

			if( event.motion.state != 0 )
			{
				const FPoint mousePos = FPoint {
					(float)event.motion.x,
					(float)event.motion.y } - mouseOffsetEased - cam;
				//enemyProjectiles.push_back( mousePos );
				SpawnProjectile( mousePos );
			}
		}
		else if( event.type == SDL_MOUSEBUTTONUP )
		{
			//mouseOffset = { 0, 0 };
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

void ShooterState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
	const float travelDist = deltaT * 60.0f;

	progress += travelDist;

	cam.x -= travelDist;

	player.x += travelDist;
	player.y += (((int)(player.x/50))%2)
		? travelDist*2
		: -travelDist*2;

	for( auto & e : enemies )
		e.x += travelDist;

	const float angle = (float)totalMSec * 0.001f;
	const float satOffsetAngle = ((float)(M_PI*2)) / (float)satCount;
	for( int i = 0; i < satCount; ++i )
	{
		sat[i].x = cos( angle + satOffsetAngle * (float)i ) * (-150) /*- sin(angle) * (100)*/ + (player.x + player.w / 2);
		sat[i].y = sin( angle + satOffsetAngle * (float)i ) * (-150) /*+ cos(angle) * (100)*/ + (player.y + player.h / 2);
	}

	for( auto & p : enemyProjectiles )
	{
		p.x -= deltaT * 300.f;
	}

	for( auto & p : myProjectiles )
	{
		p.x += deltaT * 500.f;
	}

	if( shootCooldown < totalMSec )
	{
		shootCooldown += 250;
		if( shootCooldown < totalMSec )
			shootCooldown = totalMSec + 250;

		SpawnMyProjectile( FPoint { player.x + player.w,      player.y + player.h / 2 } );
		SpawnMyProjectile( FPoint { player.x + player.w - 20, player.y + player.h / 2 - 10 } );
		SpawnMyProjectile( FPoint { player.x + player.w - 20, player.y + player.h / 2 + 10 } );
		for( int i = 0; i < satCount; ++i )
		{
			SpawnMyProjectile( sat[i] );
		}
	}

	const Rect playerRect = { (int)player.x, (int)player.y, (int)player.w, (int)player.h };

	for( auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); ++it )
	{
		auto & p = *it;

		if( !IsProjectileAlive( it ) )
			continue;

		const Point pos = { (int)p.x, (int)p.y };
		const bool isJustOutOfLevel = p.x < 0;
		const bool isPlayerHit = SDL_PointInRect( &pos, &playerRect );

		bool isPlayerExtraHit = false;
		for( int i = 0; i < satCount; ++i )
		{
			const float dx = p.x - sat[i].x;
			const float dy = p.y - sat[i].y;
			const float dist_squared = dx*dx + dy*dy;
			isPlayerExtraHit |= dist_squared < (satRadius * satRadius);

			//const bool isPlayerExtraHit2 = SDL_PointInRect( &pos, &satRect );
		}

		if( isPlayerHit )
		{
			// TODO: Damage the player
		}

		if( isPlayerHit || isPlayerExtraHit || isJustOutOfLevel )
		{
			RetireProjectile( it );
		}
	}

	for( auto it = myProjectiles.begin(); it != myProjectiles.end(); ++it )
	{
		auto & p = *it;

		if( !IsProjectileAlive( it ) )
			continue;

		const Point pos = { (int)p.x, (int)p.y };
		const bool isJustOutOfLevel = p.x > progress + game.GetWindowSize().x * 2;
		if( isJustOutOfLevel )
		{
			RetireMyProjectile( it );
			continue;
		}

		for( auto & e : enemies )
		{
			const Rect enemyRect = { (int)e.x, (int)e.y, (int)e.w, (int)e.h };
			const bool isEnemyHit = SDL_PointInRect( &pos, &enemyRect );
			if( isEnemyHit )
			{
				// TODO: Damage the enemy

				RetireMyProjectile( it );
				break;
			}
		}
	}

	CameraState::Update(frame, totalMSec, deltaT);
}

template <typename T>
constexpr bool isPowerOfTwo( T v )
{
	// v == 2`x
	static_assert( std::is_integral_v<T>, "Integral type required" );
	return v && ((v & (v - 1)) == 0);
}

template <int N, typename T>
T roundUpMultiple( T v )
{
	static_assert( N > 0, "The multiple N must be greater than 0" );

	IfDebug
		if constexpr( std::is_signed_v<T> )
			SDL_assert( v >= 0 );

	if constexpr( isPowerOfTwo( N ) )
		return (v + (N - 1)) & -N;
	else
		return ((v + (N - 1)) / N) * N;
}

void DrawCircle( SDL_Renderer * renderer, SDL_Point center, int radius )
{
	const int arrSize = roundUpMultiple<8>( radius * 8 * 35 / 49 ); // 35 / 49 is a slightly biased approximation of 1/sqrt(2)
	SDL_Point points[arrSize];
	int       drawCount = 0;

	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while( x >= y )
	{
		// Each of the following renders an octant of the circle
		points[drawCount+0] = center + SDL_Point { +x, -y };
		points[drawCount+1] = center + SDL_Point { +x, +y };
		points[drawCount+2] = center + SDL_Point { -x, -y };
		points[drawCount+3] = center + SDL_Point { -x, +y };
		points[drawCount+4] = center + SDL_Point { +y, -x };
		points[drawCount+5] = center + SDL_Point { +y, +x };
		points[drawCount+6] = center + SDL_Point { -y, -x };
		points[drawCount+7] = center + SDL_Point { -y, +x };

		drawCount += 8;

		if( error <= 0 )
		{
			++y;
			error += ty;
			ty += 2;
		}

		if( error > 0 )
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}

	SDL_RenderDrawPoints( renderer, points, drawCount );
}

/*std::vector<SDL_Point> PixelizeCircle( SDL_Point center, int radius )
{
	std::vector<SDL_Point> points;
	const int arrSize = roundUpMultiple<8>( radius * 8 * 35 / 49 ); // 35 / 49 is a slightly biased approximation of 1/sqrt(2)
	points.reserve( arrSize );

	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while( x >= y )
	{
		// Each of the following renders an octant of the circle
		points.push_back( { center.x + x, center.y - y } );
		points.push_back( { center.x + x, center.y + y } );
		points.push_back( { center.x - x, center.y - y } );
		points.push_back( { center.x - x, center.y + y } );
		points.push_back( { center.x + y, center.y - x } );
		points.push_back( { center.x + y, center.y + x } );
		points.push_back( { center.x - y, center.y - x } );
		points.push_back( { center.x - y, center.y + x } );

		if( error <= 0 )
		{
			++y;
			error += ty;
			ty += 2;
		}

		if( error > 0 )
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}

	return points;
}*/

template <int X, int Y>
Point unpack( int val ) // LtR, TtB
{
	return { val % X, (val / X) % Y };
}

void ShooterState::Render( const u32 frame, u32 totalMSec, const float deltaT )
{
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

	for( int i = 0; i <= 2; ++i ) // The first 3 layers, rendered back to front
	{
		RenderLayer(winSize, fluxCam, i);
	}

	{
		constexpr const Point pivot   = { 10, 45 };
		constexpr const Point texSize = { 10, 26 };
		constexpr const Rect  src     = {
			texSize.x,
			texSize.y,
			texSize.x,
			texSize.y };
		//for( auto & p : enemyProjectiles )
		for( auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); ++it )
		{
			auto & p = *it;

			if( !IsProjectileAlive( it ) )
				continue;

			const Point index      = unpack<10, 6>( frame + p.y );
			const Rect  srcIndexed = src * index;
			const Rect  pos        = {
				(int)(p.x + fluxCam.x) - pivot.x/2,
				(int)(p.y + fluxCam.y) - pivot.y,
				texSize.x * 2,
				texSize.y * 2 };
			SDL_RenderCopyEx( render, projectile[0], &srcIndexed, &pos, 90, &pivot, SDL_RendererFlip::SDL_FLIP_NONE );
			//SDL_RenderCopy( render, projectile[2], &src, &pos );
		}
	}

	{
		constexpr const Point pivot   = { 10, 9 };
		constexpr const Point texSize = { 65, 9 };
		constexpr const Rect  src     = {
			texSize.x,
			texSize.y,
			texSize.x,
			texSize.y };
		for( auto it = myProjectiles.begin(); it != myProjectiles.end(); ++it )
		{
			auto & p = *it;

			if( !IsProjectileAlive( it ) )
				continue;

			const Point index      = unpack<10, 6>( frame + p.y );
			const Rect  srcIndexed = src * index;
			const Rect  pos        = {
				(int)(p.x + fluxCam.x) - pivot.x/2,
				(int)(p.y + fluxCam.y) - pivot.y,
				texSize.x * 2,
				texSize.y * 2 };
			SDL_RenderCopyEx( render, projectile[2], &srcIndexed, &pos, 180, &pivot, SDL_RendererFlip::SDL_FLIP_NONE );
			//SDL_RenderCopy( render, projectile[2], &src, &pos );
		}
	}

	// Draw player
	SDL_SetRenderDrawBlendMode( render, SDL_BLENDMODE_BLEND );
	SDL_SetRenderDrawColor( render, 0, 255, 0, 127 );

	//const Point fluxCamI = { (int)fluxCam.x, (int)fluxCam.y };
	const FRect pos = player + fluxCam;
	SDL_RenderFillRectF( render, &pos );

	// Draw player satellites
	SDL_SetRenderDrawBlendMode( render, SDL_BLENDMODE_NONE );
	SDL_SetRenderDrawColor( render, 0, 255, 255, 255 );

	for( int i = 0; i < satCount; ++i )
	{
		const FPoint sat2Pos = sat[i] + fluxCam;
		DrawCircle( render, Point { (int)(sat2Pos.x), (int)(sat2Pos.y) }, satRadius );
	}

	// Draw enemies
	SDL_SetRenderDrawBlendMode( render, SDL_BLENDMODE_BLEND );
	SDL_SetRenderDrawColor( render, 255, 0, 0, 127 );

	for(auto & e : enemies)
	{
		//const Point fluxCamI = { (int)fluxCam.x, (int)fluxCam.y };
		const FRect pos = e + fluxCam;
		SDL_RenderFillRectF( render, &pos );
	}

	RenderLayer( winSize, fluxCam, 3 );
}

bool ShooterState::IsProjectileAlive( const Vector<FPoint>::iterator & it ) const
{
	return isfinite( it->x );
}

void ShooterState::SpawnProjectile( const FPoint pos )
{
	if( numDeadEnemyProj > 20 )
	{
		DebugOnly( int count = 0; )
		while( IsProjectileAlive( enemyProjReuse ) )
		{
			DebugOnly( ++count; )
			++enemyProjReuse;
			if( enemyProjReuse == enemyProjectiles.end() ) [[unlikely]]
			{
				DebugOnly( cout << "seeking wrapped around" << endl; )
				enemyProjReuse = enemyProjectiles.begin();
			}
		}
		DebugOnly( cout << "seeked for " << count << " entries in enemyProjectiles until a reusable was found" << endl; )
		*enemyProjReuse = pos;
		--numDeadEnemyProj;
	}
	else
	{
		const uint oldCapa = enemyProjectiles.capacity();

		enemyProjectiles.push_back( pos );

		const uint newCapa = enemyProjectiles.capacity();

		// Determine if iterators were invalidated
		if( oldCapa != newCapa ) [[unlikely]]
		{
			cout << oldCapa << " : oc / nc : " << newCapa << endl;
			enemyProjReuse = enemyProjectiles.begin();
		}
	}
}

void ShooterState::SpawnMyProjectile( const FPoint pos )
{
	if( numDeadMyProj > 20 )
	{
		DebugOnly( int count = 0; )
		while( IsProjectileAlive( myProjReuse ) )
		{
			DebugOnly( ++count; )
			++myProjReuse;
			if( myProjReuse == myProjectiles.end() ) [[unlikely]]
			{
				DebugOnly( cout << "seeking wrapped around" << endl; )
				myProjReuse = myProjectiles.begin();
			}
		}
		DebugOnly( cout << "seeked for " << count << " entries in myProjectiles until a reusable was found" << endl; )
		*myProjReuse = pos;
		--numDeadMyProj;
	}
	else
	{
		const uint oldCapa = myProjectiles.capacity();

		myProjectiles.push_back( pos );

		const uint newCapa = myProjectiles.capacity();

		// Determine if iterators were invalidated
		if( oldCapa != newCapa ) [[unlikely]]
		{
			cout << oldCapa << " : oc / nc : " << newCapa << endl;
			myProjReuse = myProjectiles.begin();
		}
	}
}

void ShooterState::RetireProjectile( const Vector<FPoint>::iterator & it )
{
	it->x = INFINITY;
	enemyProjReuse = it;
	++numDeadEnemyProj;
}

void ShooterState::RetireMyProjectile( const Vector<FPoint>::iterator & it )
{
	it->x = -INFINITY;
	myProjReuse = it;
	++numDeadMyProj;
}
