#pragma once

#include "global.h"

class Game;
class GameState;

class Game
{
protected:
	Window   * window;
	Renderer * render;

	bool isRunning = true;
	u32  frame     = 0;

	int currentStateIdx = -1;
	int nextStateIdx    = -1;

	GameState * currentState = nullptr;
	Vector<GameState *> allStates;

    Point windowSize;

public:
	[[nodiscard]]
	bool IsRunning() const { return isRunning; }

	explicit Game(
		const char * windowTitle = "SDL Game",
		const Point  windowSize  = Point { 1024, 720 },
		const bool   vSync       = true );
	virtual ~Game();

	virtual bool HandleEvent( const Event event );
	virtual int Run();

	virtual void SetNextState( int index ) { nextStateIdx = index; }

    Point getWindowSize(){ return windowSize; }

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

class GameState
{
protected:
	Game     & game;
	Renderer * render;

public:
	GameState( Game & game, Renderer * render );
	virtual ~GameState() = default;

	[[nodiscard]]
	virtual bool IsFPSLimited() const { return true; }

	virtual void Init() {}
	virtual void UnInit() {}

	virtual void Events( const u32 frame, const u32 totalMSec, const float deltaT ) = 0;
	virtual void Update( const u32 frame, const u32 totalMSec, const float deltaT ) = 0;
	virtual void Render( const u32 frame, const u32 totalMSec, const float deltaT ) = 0;
};
