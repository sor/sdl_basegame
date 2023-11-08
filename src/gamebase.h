#pragma once

#include <global.h>

class Game;
class GameState;

class Game
{
protected:
	Window    *         window;
	Renderer  *         renderer;
	GameState *         currentState = nullptr;
	Vector<GameState *> allStates;

	Point windowSize       = { 1, 1 };
	u32   framesSinceStart = 0;
	int   currentStateIdx  = -1;
	int   nextStateIdx     = -1;
	bool  isRunning        = true;

public:
	[[nodiscard]]       bool    IsRunning()     const { return isRunning; }
	[[nodiscard]]       Point & GetWindowSize()       { return windowSize; }
	[[nodiscard]] const Point & GetWindowSize() const { return windowSize; }

	explicit Game(
		const char * windowTitle   = "SDL Game",
		const Point  requestedSize = Point { 1024, 720 },
		const bool   vSync         = true );
	Game(              const Game &  ) = delete;
	Game(                    Game && ) = delete;
	Game &  operator=( const Game &  ) = delete;
	Game && operator=(       Game && ) = delete;
	virtual ~Game();

	virtual bool HandleEvent( const Event & event );
	virtual void Input();
	virtual void Update( const u64 msSinceStart, const float deltaT       );
	virtual void Render( const u64 msSinceStart, const float deltaTNeeded );
	virtual int Run();

	virtual void SetNextState( int index ) { nextStateIdx = index; }

protected:
	virtual void ActivateNextState();


	/// Performance
public:
	// All output msec per frame, see: http://renderingpipeline.com/2013/02/fps-vs-msecframe/
	enum class PerformanceDrawMode
	{
		None,
		Title,      // averaged and only updated every 250ms
		OStream,    // averaged and only updated every 1000ms
	};

	void SetPerfDrawMode( PerformanceDrawMode mode ) noexcept { perfDrawMode = mode; }

protected:
	PerformanceDrawMode perfDrawMode = PerformanceDrawMode::None;
	u32 lastPerfInfoFrame = 0;
	TimePoint lastPerfInfoTime = Clock::now();
	Duration accumulatedNeeded = Duration::zero();

	[[nodiscard]]
	float AverageMSecPerFrame() const;
	void ResetPerformanceInfo( const TimePoint current );
	void OutputPerformanceInfo( const TimePoint current, const Duration needed );
};

// TODO: u32 msSinceStart does overflow after ~49 days, ignore for now, Game already has u64
class GameState
{
protected:
	Game     & game;
	Renderer * renderer;

public:
	[[nodiscard]] virtual Game & GetGame()       const { return game; }
	[[nodiscard]] virtual bool   IsFPSLimited()  const { return true; }
	[[nodiscard]] virtual Color  GetClearColor() const { return Color { 0, 0, 0, SDL_ALPHA_OPAQUE }; }

	// ctor
	explicit GameState( Game && game_, Renderer * render_ ) = delete; // prevent taking an rvalue
	explicit GameState( Game &  game_, Renderer * render_ )
		: game( game_ ),
		  renderer( render_ )
	{}
	GameState(              const GameState &  ) = delete;
	GameState(                    GameState && ) = delete;
	GameState &  operator=( const GameState &  ) = delete;
	GameState && operator=(       GameState && ) = delete;
	virtual ~GameState() = default;

	virtual void Init() {}
	virtual void UnInit() {}

	// Returns a bool to know if the Event was handled
	// maybe useful in the future if GameStates could be stacked (not possible yet)
	virtual bool HandleEvent( const Event & event ) = 0;
	// This is mostly being replaced by HandleEvent, only use for stateful inputs
	virtual void Input() {};
	virtual void Update( const u32 framesSinceStart, const u32 msSinceStart, const float deltaT ) = 0;
	virtual void Render( const u32 framesSinceStart, const u32 msSinceStart, const float deltaT ) = 0;
};
