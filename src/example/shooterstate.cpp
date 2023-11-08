#include "examplegame.h"

void ShooterState::Init()
{
	CameraState::Init();

	if( !projectile[0] )
	{
		projectile[0] = IMG_LoadTexture( renderer, BasePath "asset/graphic/fire.png"   );
		projectile[1] = IMG_LoadTexture( renderer, BasePath "asset/graphic/ice.png"    );
		projectile[2] = IMG_LoadTexture( renderer, BasePath "asset/graphic/poison.png" );

		SDL_SetTextureColorMod( projectile[2], 191, 191, 191 );
	}

	if( !sound )
	{
		sound = Mix_LoadWAV( BasePath "asset/sound/pew.wav" );
		if( !sound )
			print( stderr, "Mix_LoadWAV failed: {}\n", Mix_GetError() );
	}
}

void ShooterState::UnInit()
{
	// TODO
}

bool ShooterState::HandleEvent( const Event & event )
{
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
		if( event.key.keysym.scancode == SDL_SCANCODE_F7 ) isEased    = !isEased;
		if( event.key.keysym.scancode == SDL_SCANCODE_F8 ) isFlux     = !isFlux;

		return true; // Not 100% correct
	}
	else if( event.type == SDL_MOUSEBUTTONDOWN )
	{
		const FPoint mousePos = FPoint {
			(float)event.button.x,
			(float)event.button.y } - mouseOffsetEased - cam;
		//enemyProjectiles.push_back( mousePos );
		//rvProjectiles.Spawn(mousePos);
		//rvProjectiles.Spawn({1,1});

		// This is spawning a projectile inside Update()
		spawnProjectileAt = mousePos;

		return true;
	}
	else if( event.type == SDL_MOUSEMOTION )
	{
		const FPoint halfWinSize = toF( game.GetWindowSize() / 2 );
		const FPoint mousePos    = { (float)event.motion.x, (float)event.motion.y };

		mouseOffset = halfWinSize - mousePos;

		if( event.motion.state != 0 )
		{
			const FPoint mousePosOffsetted = FPoint {
				(float)event.motion.x,
				(float)event.motion.y } - mouseOffsetEased - cam;
			//enemyProjectiles.push_back( mousePosOffsetted );

			// This is spawning a projectile inside Update()
			spawnProjectileAt = mousePosOffsetted;
		}

		return true;
	}
	else if( event.type == SDL_MOUSEBUTTONUP )
	{
		//mouseOffset = { 0, 0 };

		return true;
	}
	else
	{
		return false;
	}
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

	if( spawnProjectileAt.x >= 0
	 && spawnProjectileAt.y >= 0 )
	{
		SpawnEnemyProjectile( spawnProjectileAt );

		if( spawnProjectileSoundCD < totalMSec )
		{
			Mix_PlayChannel( -1, sound, 0 );
			spawnProjectileSoundCD = totalMSec + 35;
		}

		spawnProjectileAt = { -1, -1 };
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

		SpawnMyProjectile( FPoint { player.x + player.w,      player.y + player.h / 2      } );
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

		const Point pos              = { (int)p.x, (int)p.y };
		const bool  isJustOutOfLevel = p.x < 0;
		const bool  isPlayerHit      = SDL_PointInRect( &pos, &playerRect );

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
			const Rect enemyRect  = { (int)e.x, (int)e.y, (int)e.w, (int)e.h };
			const bool isEnemyHit = SDL_PointInRect( &pos, &enemyRect );
			if( isEnemyHit )
			{
				// TODO: Damage the enemy

				RetireMyProjectile( it );
				break;
			}
		}
	}

	CameraState::Update( frame, totalMSec, deltaT );
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

Vector<SDL_Point> PixelizeCircle( SDL_Point center, int radius )
{
	Vector<SDL_Point> points;
	const int arrSize = roundUpMultiple<8>( radius * 8 * 35 / 49 ); // 35 / 49 is a slightly biased approximation of 1/sqrt(2)
	points.reserve( arrSize );

	const i32 diameter = (radius * 2);

	i32 x = (radius - 1);
	i32 y = 0;
	i32 tx = 1;
	i32 ty = 1;
	i32 error = (tx - diameter);

	while( x >= y )
	{
		// Each of the following renders an octant of the circle
		points.push_back( center + SDL_Point { +x, -y } );
		points.push_back( center + SDL_Point { +x, +y } );
		points.push_back( center + SDL_Point { -x, -y } );
		points.push_back( center + SDL_Point { -x, +y } );
		points.push_back( center + SDL_Point { +y, -x } );
		points.push_back( center + SDL_Point { +y, +x } );
		points.push_back( center + SDL_Point { -y, -x } );
		points.push_back( center + SDL_Point { -y, +x } );

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

	return points; // NRVO will make this good, fingers crossed
}

void DrawCircle( SDL_Renderer* renderer, const Vector<SDL_Point> & points )
{
	SDL_RenderDrawPoints( renderer, points.data(), points.size() );
}

void DrawCircle( SDL_Renderer* renderer, SDL_Point center, int radius )
{
	const Vector<SDL_Point> points = PixelizeCircle( center, radius );
	DrawCircle( renderer, points );
}

template <int X, int Y>
Point unpack( int val ) // LtR, TtB - Left to Right, Top to Bottom
{
	return { val % X, (val / X) % Y };
}

void ShooterState::Render( const u32 frame, u32 totalMSec, const float deltaT )
{
	// Try the limits, moments before wraparound
	//totalMSec += 2147470000u + 2147480000u;

	const Point  winSize = game.GetWindowSize();
	const FPoint fluxCam = CalcFluxCam( totalMSec );

	for( int i = 0; i <= 2; ++i ) // Only the first 3 layers, rendered back to front
	{
		RenderLayer( winSize, fluxCam, i );
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
			SDL_RenderCopyEx( renderer, projectile[0], &srcIndexed, &pos, 90, &pivot, SDL_RendererFlip::SDL_FLIP_NONE );
			//SDL_RenderCopy( renderer, projectile[2], &src, &pos );
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
			SDL_RenderCopyEx( renderer, projectile[2], &srcIndexed, &pos, 180, &pivot, SDL_RendererFlip::SDL_FLIP_NONE );
			//SDL_RenderCopy( renderer, projectile[2], &src, &pos );
		}
	}

	// Draw player
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
	SDL_SetRenderDrawColor( renderer, 0, 255, 0, 127 );

	//const Point fluxCamI = { (int)fluxCam.x, (int)fluxCam.y };
	const FRect pos = player + fluxCam;
	SDL_RenderFillRectF( renderer, &pos );

	// Draw player satellites
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_NONE );
	SDL_SetRenderDrawColor( renderer, 0, 255, 255, 255 );

	for( int i = 0; i < satCount; ++i )
	{
		const FPoint sat2Pos = sat[i] + fluxCam;
		DrawCircle( renderer, Point { (int)(sat2Pos.x), (int)(sat2Pos.y) }, satRadius );
	}

	// Draw enemies
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 127 );

	for(auto & e : enemies)
	{
		//const Point fluxCamI = { (int)fluxCam.x, (int)fluxCam.y };
		const FRect posOffsetted = e + fluxCam;
		SDL_RenderFillRectF( renderer, &posOffsetted );
	}

	for( int i = 3; i <= 3; ++i ) // Render the last 1 layers, rendered back to front
		RenderLayer( winSize, fluxCam, i );
}

bool ShooterState::IsProjectileAlive( const Vector<FPoint>::iterator & it ) const
{
	return isfinite( it->x );
}

void ShooterState::SpawnEnemyProjectile( const FPoint pos )
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
				DebugOnly( print( "seeking wrapped around\n" ); )
				enemyProjReuse = enemyProjectiles.begin();
			}
		}
		DebugOnly( print( "seeked for {} entries in enemyProjectiles until a reusable was found\n", count ); )
		*enemyProjReuse = pos;
		--numDeadEnemyProj;
	}
	else
	{
		const usize oldCapa = enemyProjectiles.capacity();

		enemyProjectiles.push_back( pos );

		const usize newCapa = enemyProjectiles.capacity();

		// Determine if iterators were invalidated
		if( oldCapa != newCapa ) [[unlikely]]
		{
			DebugOnly( print( "oc: {} nc: {}\n", oldCapa, newCapa ); )
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
				DebugOnly( print( "seeking wrapped around" ); )
				myProjReuse = myProjectiles.begin();
			}
		}
		DebugOnly( print( "seeked for {} entries in myProjectiles until a reusable was found\n", count ); )
		*myProjReuse = pos;
		--numDeadMyProj;
	}
	else
	{
		const usize oldCapa = myProjectiles.capacity();

		myProjectiles.push_back( pos );

		const usize newCapa = myProjectiles.capacity();

		// Determine if iterators were invalidated
		if( oldCapa != newCapa ) [[unlikely]]
		{
			DebugOnly( print( "oc: {} nc: {}\n", oldCapa, newCapa ); )
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
