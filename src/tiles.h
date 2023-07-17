#pragma once

#include <global.h>
#include <recthelper.h>

#include "example/hsnr64.h"

//#define TILE_INDEX_MAX_4096

// The Tileset to use is specified by the TileLayer
// Uses 3 or 3.5 (rounded up to 4) bytes, without the bitfield-micro-optimization this would be 8 bytes
// The 3 byte version only uses a 12 bit index, meaning 4096 indices (e.g. 64^2) are possible
// The 4 byte version uses a 16 bit index, meaning 64k indices (e.g. 256^2) are possible
struct Tile
{
#ifdef TILE_INDEX_MAX_4096
	u8   index1         :8  = 0;        // index1 == 0 && index2 == 0 means don't draw
	u8   index2         :4  = 0;        // index == index1 + index2 * 256
#else
	u16  index1         :16 = 0;        // index1 == 0 means don't draw
#endif
	bool horizontalFlip :1  = false;    // SDL_RendererFlip::SDL_FLIP_HORIZONTAL
	bool verticalFlip   :1  = false;    // SDL_RendererFlip::SDL_FLIP_VERTICAL
	bool rotate90       :1  = false;
	bool rotate180      :1  = false;
	u8   color          :6  = 0;        // SDL_SetTextureColorMod, color == 63 means don't draw
	u8   alpha          :2  = 0;        // SDL_SetTextureAlphaMod // 0 = 25%, 1 = 50%, 2 = 75%, 3 = 100%

#ifdef TILE_INDEX_MAX_4096
	[[nodiscard]] inline u16  index() const  { return index1 + index2 * 256; }
	              inline void index( u16 i ) { index1 = i % 256; index2 = i / 256; }
#else
	[[nodiscard]] inline u16  index() const  { return index1; }
	              inline void index( u16 i ) { index1 = i; }
#endif
};
#ifdef TILE_INDEX_MAX_4096
static_assert( sizeof(Tile) == 3 );
#else
static_assert( sizeof(Tile) == 4 );
#endif

inline
void DrawTile( Renderer * renderer, Texture * tex, const Tile & curr_tile, const Tile & last_tile, const FPoint & center )
{
	// An index of 0 means: Do not render anything, which fills in for alpha 0% (which therefore does not exist as value in alpha)
	// There needs to be a sentinel last_tile for the first call, which has color and alpha set to the default values
	assert( curr_tile.index() != 0 ); // "Prevent entering this function on index == 0"
	assert( last_tile.index() != 0 ); // "The last_tile can also not be index == 0, pass in the one before that"

	// color == 0 also means do not draw
	if( curr_tile.color == 0 )
	{
		return;
	}
	else if( curr_tile.color != last_tile.color )
	{
		/*
		Surface     * surf;
		PixelFormat * format = surf->format;
		Palette     * pal    = format->palette;                 assert( curr_tile.index() < pal->ncolors );
		Color       & col    = pal->colors[curr_tile.index()];
		*/
		const Color & col    = sor::hsnr64::Palette[curr_tile.color];
		SDL_SetTextureColorMod( tex, col.r, col.g, col.b );
	}

	if( curr_tile.alpha != last_tile.alpha )
	{
		// Variable alpha ends up with these 4 possible values
		//  0: ~25% = 0b00'11'11'11 =  63(/255)
		//  1: ~50% = 0b01'11'11'11 = 127(/255)
		//  2: ~75% = 0b10'11'11'11 = 191(/255)
		//  3: 100% = 0b11'11'11'11 = 255(/255)
		u8 alpha = curr_tile.alpha << 6 | 0b11'11'11;
		SDL_SetTextureAlphaMod( tex, alpha );
	}
	const u8 tileSizeX = 16, tileSizeY = 16;
	const u16 stride   = 64;
	const u16 idx = curr_tile.index();
	const Rect             src   = { (idx / stride) * tileSizeX, (idx % stride) * tileSizeY, tileSizeX, tileSizeY };
	const FRect            dst   = { 0, 0, tileSizeX, tileSizeY };
	const double           angle = curr_tile.rotate90 * 90 + curr_tile.rotate180 * 180.0;
	const SDL_RendererFlip flip  = (SDL_RendererFlip) (curr_tile.horizontalFlip * SDL_FLIP_HORIZONTAL + curr_tile.verticalFlip * SDL_FLIP_VERTICAL);

	SDL_RenderCopyExF( renderer, tex, &src, &dst, angle, &center, flip );
}

struct TileInfo
{
	// Collider, Hitbox, Hurtbox (does it hurt you or others?)
	// Bounding volumes/areas
	u16     bitset;
	Point   orderPivot;  // might be interesting for DrawingOrder::BackToFront
};

// Should be able to be used multiple times
struct TileSet
{
	Texture *       texture;
	Point           textureSize = {  512,  1024 };     // Refering to the texture x,y = size of one tile; w,h = amount of tiles
	Point           tileSize    = {   16,    16 };     // Size of a Tile in pixels
	Point           tileCount   = {   32,    64 };     // Refering to the texture x,y = size of one tile; w,h = amount of tiles
//	Point           startOffset = {    0,     0 };     // Empty space left/top of the first Tile
//	Point           margin      = {    0,     0 };     // Empty space between the Tiles
};

// Uniform Tiling of 2D Space
// TODO: cacheability of the whole layer
struct TileMap // TileLayer
{
	constexpr static float  halfSize  = 0.5f;
	constexpr static FPoint halfPoint = { 0.5f, 0.5f };

	Vector<Tile>    tiles;
	TileSet *       tileSet;
	FPoint          tileDist    = { 16.0f, 16.0f };     // This should usually be the same as tileSet.tileSize, but for an overlapping TileSet this can for example be lower
	FPoint          tileOffset  = {  0.0f,  0.0f };     // Offset from the zero coordinate
	FPoint          sizeScale   = {  1.0f,  1.0f };     // Output size scaling
	FPoint          scrollScale = {  1.0f,  1.0f };     // Parallax scroll scaling
	FPoint          center;      //= toF( tileSet->tileSize ) * sizeScale * halfSize;
	int             stride;
	SDL_BlendMode   blendMode   = SDL_BLENDMODE_BLEND;  // SDL_SetTextureBlendMode
	SDL_ScaleMode   scaleMode   = SDL_ScaleModeNearest; // SDL_SetTextureScaleMode -OR- SDL_HINT_RENDER_SCALE_QUALITY 0, 1, 2
//	DrawingOrder    order       = DrawingOrder::DontCare;
//	bool            enabled     = true;
};

struct MultiTileMap // MultiLayer
{
	Vector<TileMap> layers;
	Vector<bool>    enabledLayers;
};

/*
// TODO: Create a 2D vector, that supports growing in 2 dimensions.
// Consists of single-alloc-sectors which are a quadratic area each, e.g. 32x32x4bytes = 4096
template <typename T, size_t S>
class Vector2D
{
//	static_assert( isPowerOfTwo( S ) );
	static_assert( S * S * sizeof( T ) <= 4096 );
	Vector<Array<T, S * S>> sectors;

	// method to grow x direction -> just append on the end
	// method to grow y direction -> insert at 1..n * stride
};

inline Vector2D<Tile,32> v2d;
*/
