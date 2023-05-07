#include "gamebase.h"

Game::Game( const char * windowTitle, const Point windowSize, const bool vSync )
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) )
	{
		cerr << "SDL_Init failed: " << SDL_GetError() << endl;
		exit( 1 );
	}

	if( TTF_Init() )
	{
		cerr << "TTF_Init failed: " << TTF_GetError() << endl;
		exit( 2 );
	}

	constexpr IMG_InitFlags imgFlags = (IMG_InitFlags) (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	if( IMG_Init( imgFlags ) != imgFlags )
	{
		cerr << "IMG_Init failed: " << IMG_GetError() << endl;
		exit( 3 );
	}

	constexpr MIX_InitFlags mixFlags = (MIX_InitFlags) (MIX_INIT_MP3 | MIX_INIT_OGG);
	if( Mix_Init( mixFlags ) != mixFlags )
	{
		cerr << "Mix_Init failed: " << Mix_GetError() << endl;
		exit( 4 );
	}

	if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) < 0 )
	{
		cerr << "Mix_OpenAudio failed: " << Mix_GetError() << endl;
		exit( 5 );
	}

	window = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowSize.x,
		windowSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	if( window == nullptr )
	{
		cerr << "Window could not be created: " << SDL_GetError() << endl;
		exit( 4 );
	}

	render = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
		| (vSync
		   ? SDL_RENDERER_PRESENTVSYNC
		   : 0) );

	if( render == nullptr )
	{
		cerr << "Renderer could not be created: " << SDL_GetError() << endl;
		exit( 5 );
	}

	allStates.reserve( 10 );
	std::fill( allStates.begin(), allStates.end(), nullptr );
}

Game::~Game()
{
	for( GameState * state : allStates )
		delete state;

	if( render != nullptr )
		SDL_DestroyRenderer( render );

	if( window != nullptr )
		SDL_DestroyWindow( window );

	IMG_Quit();

	if( TTF_WasInit() )
		TTF_Quit();

	if( SDL_WasInit( 0 ) )
		SDL_Quit();
}

bool Game::HandleEvent( const Event event )
{
	switch( event.type )
	{
		case SDL_QUIT:
			isRunning = false;
			return true;
			break;

		case SDL_KEYDOWN:
		{
			auto & key_event = event.key;
			Keysym what_key = key_event.keysym;

			if( (what_key.mod & KMOD_ALT) &&
			    (what_key.scancode == SDL_SCANCODE_F4) )
			{
				Event next_event; // = { .type = SDL_QUIT };
				next_event.type = SDL_QUIT;
				SDL_PushEvent( &next_event );
				return true;
			}
			break;
		}
	}
	return false;
}

int Game::Run()
{
	SDL_assert( nextStateIdx >= 0 );

	Duration deltaT = Duration::zero();
	Duration deltaTNeeded = Duration::zero();   // How much time was really necessary
	TimePoint start;

	while( IsRunning() )
	{
		start = Clock::now();

		const float deltaTF = std::chrono::duration<float>( deltaT ).count();
		const float deltaTFNeeded = std::chrono::duration<float>( deltaTNeeded ).count();

		OutputPerformanceInfo( start, deltaTNeeded );

		ActivateNextState();

		// The difference to last frame is usually 16-17 at 60Hz, 10 at 100Hz, 8-9 at 120Hz, 6-*7* at 144Hz
		const u32 totalMSec = SDL_GetTicks();

		SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);


		currentState->Events( frame, totalMSec, deltaTF );

		currentState->Update( frame, totalMSec, deltaTF );

		const Color clear = currentState->GetClearColor();
		if( clear.a != SDL_ALPHA_TRANSPARENT)
		{
			SDL_SetRenderDrawColor( render, clear.r, clear.g, clear.b, clear.a );
			SDL_RenderClear( render );
		}
		currentState->Render( frame, totalMSec, deltaTFNeeded );
		SDL_RenderPresent( render );


		deltaTNeeded = Clock::now() - start;

		if( currentState->IsFPSLimited() )
		{
			using namespace std::chrono_literals;

			const Duration dur = std::max( Duration::zero(), 16ms - deltaTNeeded );
			const u32 ms = static_cast<u32>( std::chrono::duration_cast<std::chrono::milliseconds>( dur ).count() );
			SDL_Delay( ms );
			deltaT = Clock::now() - start;
		}
		else
		{
			deltaT = deltaTNeeded;
		}

		++frame;
	}
	return 0;
}

void Game::ActivateNextState()
{
	if( nextStateIdx != currentStateIdx
	    && nextStateIdx != -1 )
	{
		// Load the state or die
		if( nextStateIdx >= (int)allStates.size() || allStates[nextStateIdx] == nullptr )
		{
			cerr << "Activated out of range or nullptr state with the index: " << nextStateIdx << endl;
			exit( 11 );
		}
		else
		{
			if( currentState != nullptr )
				currentState->UnInit();

			currentStateIdx = nextStateIdx;
			currentState = allStates[currentStateIdx];
			currentState->Init();
		}
	}
}

float Game::AverageMSecPerFrame() const
{
	const u32 passedFrames = frame - lastPerfInfoFrame + 1;
	return std::chrono::duration<float>( accumulatedNeeded / passedFrames ).count() * 1000.0f;
}

void Game::ResetPerformanceInfo( const TimePoint current )
{
	lastPerfInfoFrame = frame;
	lastPerfInfoTime = current;
	accumulatedNeeded = Duration::zero();
}

void Game::OutputPerformanceInfo( const TimePoint current, const Duration needed )
{
	using namespace std::chrono_literals;

	accumulatedNeeded += needed;

	const Duration passedTime = current - lastPerfInfoTime;

	switch( perfDrawMode )
	{
		case PerformanceDrawMode::Title:
			if( passedTime > 250ms )
			{
				std::ostringstream oss;
				oss << AverageMSecPerFrame() << "ms";
				SDL_SetWindowTitle( window, oss.str().c_str() );
				ResetPerformanceInfo( current );
			}
			break;

		case PerformanceDrawMode::OStream:
			if( passedTime > 1000ms )
			{
				cout << AverageMSecPerFrame() << "ms" << endl;
				ResetPerformanceInfo( current );
			}
			break;

		case PerformanceDrawMode::None:
		default: break;
	}
}
