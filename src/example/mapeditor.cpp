#include "examplegame.h"

#include <tiles.h>

void MapEditorState::Init()
{
	GameState::Init();

	SDL_Surface * surface = IMG_Load( BasePath "asset/graphic/16x16_hsnr64.png" );
	if( surface != nullptr )
	{
		tileSet.texture     = SDL_CreateTextureFromSurface( render, surface );
		tileSet.textureSize = { surface->w, surface->h };
		tileSet.tileSize    = { 16, 16 };
		tileSet.tileCount   = tileSet.textureSize / tileSet.tileSize; // kind of unnecessary
		SDL_FreeSurface( surface );
	}

	tileMap.tileSet  = &tileSet;
	tileMap.tileDist = toF( tileMap.tileSet->tileSize );
	tileMap.center   = toF( tileMap.tileSet->tileSize ) * tileMap.sizeScale * tileMap.halfSize;
	tileMap.stride   = tileSet.tileCount.x;
}


void MapEditorState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	SDL_SetTextureBlendMode( tileSet.texture, tileMap.blendMode );
	SDL_SetTextureScaleMode( tileSet.texture, tileMap.scaleMode );

	// 10 25
	const FPoint viewOffset = { 6000,   0 };
	const Point  viewSize   = {  640, 480 };
	const Point  startIndex = toI( viewOffset / tileMap.tileDist );

	int x = 0, y = 0;
	int index = x + y * tileMap.stride;

	Tile * curr_tile = &tileMap.tiles[index];
	Tile * last_tile = curr_tile; // Should suffice as sentinel for now

	DrawTile( render, tileSet.texture, *curr_tile, *last_tile, tileMap.center );
}

// BFS mit 2 Startpunkten berechnen, braucht keine 2 kalkulationen weil er direkt bestimmt wer am nähesten dran ist

