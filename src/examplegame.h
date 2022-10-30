#pragma once

#include "global.h"

#include "gamebase.h"

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
		Point { -1, -1 },
		Point { +1, -1 },
		Point { -1, +1 },
		Point { +1, +1 },
		Point { +0, +2 },
		Point { +2, +0 },
		Point { +0, -2 },
		Point { -2, +0 },
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

class ExampleState2 final : public ExampleState
{
public:
	// ctor
	using ExampleState::ExampleState;

	void Init() override;
};

class ExampleGame final : public Game
{
public:
	ExampleGame();

	bool HandleEvent( const Event event ) override;
};
