#pragma once

#include "global.h"

#include "gamebase.h"
#include "recthelper.h"

class ExampleGame;
class ExampleState;
class PlasmaState;
class SortState;

class ExampleGame final : public Game
{
public:
	ExampleGame();

	bool HandleEvent( const Event event ) override;
};

class ExampleState : public GameState
{
protected:
	Font    * font              = nullptr;
	Texture * image             = nullptr;
	Texture * blendedText       = nullptr;
	Point     blendedTextSize   = { 0, 0 };

	static constexpr const Color white { 255, 255, 255, 255 };
	static constexpr const Color black { 0, 0, 0, 255 };

	static constexpr const Array<Point,8> shadowOffsets = {
		/*
		Point { -1, -1 },
		Point { +1, -1 },
		Point { -1, +1 },
		Point { +1, +1 },
		Point { +0, +2 },
		Point { +2, +0 },
		Point { +0, -2 },
		Point { -2, +0 },
		*/
		1_up   + 1_left,
		1_up   + 1_right,
		1_down + 1_left,
		1_down + 1_right,
		2_right,
		2_left,
		2_up,
		2_down,
	};

public:
	// ctor
	using GameState::GameState;

	void Init() override;
	void UnInit() override;

	void Events( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;
};

class PlasmaState final : public ExampleState
{
	Surface * plasmaSrf;
	Texture * plasmaTex;

	int brightness = 320;

	static constexpr const int Scale = 8;

public:
	// ctor
	using ExampleState::ExampleState;

	void Init() override;
	void UnInit() override;

	void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;
};

class SortState : public GameState
{
protected:
	struct Ball { float x,y,z,w; };

	Texture * image = nullptr;
	Vector<Ball> balls;
	bool isOrdered = false;

public:
	// ctor
	using GameState::GameState;

	void Init() override;
	void UnInit() override;

	void Events( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;
};
