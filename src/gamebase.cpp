#include "gamebase.h"

Game::Game( const char * windowTitle, const Point requestedSize, const bool vSync )
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) )
	{
		print( stderr, "SDL_Init failed: {}\n", SDL_GetError() );
		exit( 1 );
	}

	if( TTF_Init() )
	{
		print( stderr, "TTF_Init failed: {}\n", TTF_GetError() );
		exit( 2 );
	}

	constexpr IMG_InitFlags imgFlags = (IMG_InitFlags) (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	if( IMG_Init( imgFlags ) != imgFlags )
	{
		print( stderr, "IMG_Init failed: {}\n", IMG_GetError() );
		exit( 3 );
	}

	constexpr MIX_InitFlags mixFlags = (MIX_InitFlags) (MIX_INIT_MP3 | MIX_INIT_OGG);
	if( Mix_Init( mixFlags ) != mixFlags )
	{
		print( stderr, "Mix_Init failed: {}\n", Mix_GetError() );
		exit( 4 );
	}

	if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) < 0 )
	{
		print( stderr, "Mix_OpenAudio failed: {}\n", Mix_GetError() );
		exit( 5 );
	}

	ImGuiOnly( SDL_Init_ImGui(); )

	window = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		requestedSize.x,
		requestedSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	if( window == nullptr )
	{
		print( stderr, "Window could not be created: {}\n", SDL_GetError() );
		exit( 6 );
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
		print( stderr, "Renderer could not be created: {}\n", SDL_GetError() );
		exit( 7 );
	}

	ImGuiOnly( SDL_Create_ImGui( render, window ); )

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
	ImGuiOnly(
		ImGuiIO & io = ImGui::GetIO();
		ImGui_ImplSDL2_ProcessEvent( &event );
	)

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
				Event next_event = { .type = SDL_QUIT };
				SDL_PushEvent( &next_event );
				return true;
			}
			ImGuiOnly(
				else if( io.WantCaptureKeyboard )
				{
					return true;
				}
			)

			break;
		}

		case SDL_KEYUP:
			ImGuiOnly(
				if( io.WantCaptureKeyboard )
					return true;
			)

			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
			ImGuiOnly(
				if( io.WantCaptureMouse )
					return true;
			)

			break;
	}
	return false;
}

int Game::Run()
{
	SDL_assert( nextStateIdx >= 0 );

	Duration deltaT = Duration::zero();
	Duration deltaTNeeded = Duration::zero();   // How much time was really necessary
	TimePoint start;

	if( 0 ) {
		nfdchar_t * outPath = NULL;
		nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );

		if( result == NFD_OKAY ) {
			print( "Success! Path is {0}\n", outPath );
			free( outPath );
		}
	}

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
		// TODO: SDL_RenderSetScale( renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y );
		//if( clear.a != SDL_ALPHA_TRANSPARENT)
		{
			SDL_SetRenderDrawColor( render, clear.r, clear.g, clear.b, clear.a );
			SDL_RenderClear( render );
		}

		ImGuiOnly({
			ImGui_ImplSDLRenderer2_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		})
		currentState->Render( frame, totalMSec, deltaTFNeeded );
		ImGuiOnly( SDL_ImGui_Frame( render, window ) );

		ImGuiOnly({
			ImGui::Render();
			ImGui_ImplSDLRenderer2_RenderDrawData( ImGui::GetDrawData() );
		})
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
			print( stderr, "Activated out of range or nullptr state with the index: {}\n", nextStateIdx );
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
				const std::string avgms = format( "{}ms", AverageMSecPerFrame() );
				SDL_SetWindowTitle( window, avgms.c_str() );
				ResetPerformanceInfo( current );
			}
			break;

		case PerformanceDrawMode::OStream:
			if( passedTime > 1000ms )
			{
				print( "{}ms", AverageMSecPerFrame() );
				ResetPerformanceInfo( current );
			}
			break;

		case PerformanceDrawMode::None:
		default: break;
	}
}
