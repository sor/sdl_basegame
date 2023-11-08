#pragma once

#include <global.h>

#include <gamebase.h>
#include <recthelper.h>
#include <autocast_ptr.h>
#include <tiles.h>

class ExampleGame;
class IntroState;
class PlasmaState;
class SortState;
class CameraState;
class ShooterState;
class MapEditorState;

class ExampleGame final : public Game
{
public:
	ExampleGame();

	bool HandleEvent( const Event & event ) override;
};

class ExampleGameState : public GameState
{
public:
	[[nodiscard]] ExampleGame & GetGame() const override { return dynamic_cast<ExampleGame &>( GameState::GetGame() ); }

	// ctor
	using GameState::GameState;
};

class IntroState : public ExampleGameState
{
protected:
	Font    * font              = nullptr;
	Texture * image             = nullptr;
	Music   * music             = nullptr;
	Chunk   * sound             = nullptr;
	Texture * blendedText       = nullptr;
	Point     blendedTextSize   = { 0, 0 };
	u8        textmode          = 1;

	static constexpr const Color white { 255, 255, 255, SDL_ALPHA_OPAQUE };
	static constexpr const Color black {   0,   0,   0, SDL_ALPHA_OPAQUE };

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
	using ExampleGameState::ExampleGameState;

	void Init() override;
	void UnInit() override;

	bool HandleEvent( const Event & event ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;

	Color GetClearColor() const override { return Color { 255, 255, 255, SDL_ALPHA_OPAQUE }; }
};

class PlasmaState final : public IntroState
{
	Surface * plasmaSrf;
	Texture * plasmaTex;

	int brightness = 160;

	static constexpr const int Scale = 8;

public:
	// ctor
	using IntroState::IntroState;

	void Init() override;
	void UnInit() override;

	bool HandleEvent( const Event & event ) override;
	void Input() override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;
};

class SortState : public ExampleGameState
{
protected:
	struct Ball { float x,y,z,w; };

	Texture * image = nullptr;
	Vector<Ball> balls;
	bool isOrdered = false;
	bool isTransparent = false;
	bool isDarkened = false;

public:
	// ctor
	using ExampleGameState::ExampleGameState;

	void Init() override;
	void UnInit() override;

	bool HandleEvent( const Event & event ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;
};

class CameraState : public ExampleGameState
{
protected:
	Texture *    bg[4]      = { nullptr };
	Point        bgSize[4]; // Is initialized in Init()
	const FPoint bgStart[4] = {
		{ 0,    -330 },
		{ -350, -330 },
		{ -450, -900 },
		{ -800, -1500 },
	};
	const FPoint bgFactor[4] = {
		{ 0.2f, 0.3f },
		{ 0.4f, 0.45f },
		{ 0.8f, 0.8f },
		{ 1.2f, 1.2f },
	};
	bool bgIsVisible[4] = {
		true,
		true,
		true,
		true,
	};
	FPoint mouseOffset      = { 0, 0 };
	FPoint mouseOffsetEased = { 0, 0 };

	bool isInverted = false;
	bool isEased    = true;
	bool isFlux     = true;
	FPoint dir      = { 0, 0 };
	FPoint cam      = { 0, 0 };

public:
	// ctor
	using ExampleGameState::ExampleGameState;

	void Init() override;
	void UnInit() override;

	bool HandleEvent( const Event & event ) override;
	void Input() override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;

	FPoint CalcFluxCam(const u32 totalMSec) const;
	void RenderLayer(const Point winSize, const FPoint camPos, const int index) const;
};

class ShooterState : public CameraState
{
protected:
	float progress      = 0;
	u32   shootCooldown = 0;

	Chunk   * sound         = nullptr;
	Texture * projectile[4] = { nullptr };
	//ReuseFPoints rvProjectiles;

	Vector<FPoint>           enemyProjectiles;
	Vector<FPoint>::iterator enemyProjReuse;
	int                      numDeadEnemyProj = 0;

	Vector<FPoint>           myProjectiles;
	Vector<FPoint>::iterator myProjReuse;
	int                      numDeadMyProj = 0;

	Vector<FRect> enemies = {
		{ 1100, 280, 160, 160 },
		{ 1200, 500, 80, 80 },
		{ 1100, 680, 80, 80 },
	};

	FRect   player    = { 200, 530, 200, 100 };
	FPoint  sat[5]    = { { 0, 0 } };
	int     satCount  = 5;

	FPoint spawnProjectileAt;           // Valid if x and y is >= 0
	u32    spawnProjectileSoundCD = 0;  //

	static constexpr const int satRadius = 25;

public:
	// ctor
	using CameraState::CameraState;

	void Init() override;
	void UnInit() override;

	bool HandleEvent( const Event & event ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;

	[[nodiscard]]
	bool IsProjectileAlive( const Vector<FPoint>::iterator & it ) const;
	void SpawnEnemyProjectile( const FPoint pos );
	void SpawnMyProjectile( const FPoint pos );
	void RetireProjectile(  const Vector<FPoint>::iterator & it );
	void RetireMyProjectile(const Vector<FPoint>::iterator & it );
};

class MapEditorState : public ExampleGameState
{
protected:
	TileSet tileSet;
	TileMap tileMap;

	// all below might be junk

	Texture * bg[4] = { nullptr };
	Point bgSize[4];
	const FPoint bgStart[4] = {
		{ 0,    -330 },
		{ -350, -330 },
		{ -450, -900 },
		{ -800, -1500 },
	};
	const FPoint bgFactor[4] = {
		{ 0.2f, 0.3f },
		{ 0.4f, 0.45f },
		{ 0.8f, 0.8f },
		{ 1.2f, 1.2f },
	};
	bool bgIsVisible[4] = {
		true,
		true,
		true,
		true,
	};
	FPoint mouseOffset = { 0, 0 };
	FPoint mouseOffsetEased = { 0, 0 };

	bool isInverted = false;
	bool isEased = true;
	bool isFlux = true;
	FPoint cam { .x = 0, .y = 0 };

public:
	// ctor
	using ExampleGameState::ExampleGameState;

	void Init() override;
	void UnInit() override;

	template <typename E>
	void HandleSpecificEvent( const E & ev );
	bool HandleEvent( const Event & event ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;

	FPoint CalcFluxCam(const u32 totalMSec) const;
	void RenderLayer(const Point winSize, const FPoint camPos, const int index) const;
};
