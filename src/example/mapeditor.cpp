#include "examplegame.h"

#include <tiles.h>

void MapEditorState::Init()
{
	GameState::Init();

	SDL_Surface * surface = IMG_Load( BasePath "asset/graphic/16x16_hsnr64.png" );
	if( surface != nullptr )
	{
		SDL_SetColorKey( surface, true, SDL_MapRGB( surface->format, 0, 0, 0 ) );

		tileSet.texture     = SDL_CreateTextureFromSurface( renderer, surface );
		tileSet.textureSize = { surface->w, surface->h };
		tileSet.tileSize    = { 16, 16 };
		tileSet.tileCount   = tileSet.textureSize / tileSet.tileSize; // kind of unnecessary
		SDL_FreeSurface( surface );
	}

	tileMap.tileSet  = &tileSet;
	tileMap.tileDist = toF( tileMap.tileSet->tileSize );
	tileMap.center   = toF( tileMap.tileSet->tileSize ) * tileMap.sizeScale * tileMap.halfSize;
	tileMap.stride   = tileSet.tileCount.x; // needed if tiles is one dimensional
//	tileMap.scaleMode = SDL_ScaleModeBest;

	tileMap.tiles.resize( tileSet.tileCount.x * tileSet.tileCount.y );
}

void MapEditorState::UnInit()
{

}

template <typename E>
void MapEditorState::HandleSpecificEvent( const E & ev )
{
	// Not implemented by design
	// Assert here to catch unhandled events
	//assert( false );
}


bool MapEditorState::HandleEvent( const Event & ev )
{
	// TODO

	return true; // Not really correct
}

void MapEditorState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{}

void MapEditorState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	SDL_SetTextureBlendMode( tileSet.texture, tileMap.blendMode );
	SDL_SetTextureScaleMode( tileSet.texture, tileMap.scaleMode );

	SDL_SetTextureColorMod( tileSet.texture, 255, 0, 255 );
	SDL_SetTextureAlphaMod( tileSet.texture, SDL_ALPHA_OPAQUE );
	const Tile sentinelTile = Tile { 1000, false, false, false, false, 0, 3 };

	// 10 25
	const FPoint viewOffset = { 6000,   0 };
	const Point  viewSize   = {  640, 480 };
	const Point  startIndex = toI( viewOffset / tileMap.tileDist );

	int x = 0, y = 0;
	int index = x + y * tileMap.stride;

	/*const*/ Tile * curr_tile = &tileMap.tiles[index];
	const     Tile * last_tile = &sentinelTile;

	*curr_tile = Tile { 1010, rand()%2==0, rand()%2==0, false, false, (u8)(16+(rand()%6)), 1 };
	DrawTile( renderer, tileSet.texture, *curr_tile, *last_tile, tileMap.center );

	Rect  palSrc {   0, 0, 512, 512 };
	FRect palDst { 640, 0, 512, 512 };
	// Draw the Palette (the TileSet)
	SDL_RenderCopyF( renderer, tileSet.texture, &palSrc, &palDst );

}

// Wieso ist dieser Comment hier und nicht irgendwo zentral? :D
// BFS mit 2 Startpunkten berechnen, braucht keine 2 kalkulationen, weil er mit beiden spielern in der queue starten kann, deswegen direkt bestimmt wer am nähesten dran ist
