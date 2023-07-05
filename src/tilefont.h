#pragma once

#include <global.h>

#include <recthelper.h>

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

struct CharMetric
{
	u8 width    = 16;
	i8 yoff     = 0;
	i8 left [4] = { 0, 0, 0, 0 };
	i8 right[4] = { 0, 0, 0, 0 };
	u8 merge_l  = 0b0000'0000;  // pointy / round
	u8 merge_r  = 0b0000'0000;  // round  / pointy
	u8 ligature_level = 255;    // higher = less likely; both chars will be multiplied together to form the final number
};

const Array<CharMetric,128> char_metrics = {
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},
	CharMetric{},

	CharMetric{  6, 0, { 1, 0, 0, 1 }, { 1, 0, 0, 1 } },
	CharMetric{  3, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 10, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 13, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // #
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // $
	CharMetric{ 13, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 14, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  3, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // '
	CharMetric{  7, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  7, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 14, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  5, 1, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // ,
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  3, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // .
	CharMetric{ 13, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // /
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // 0
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 12, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // 9
	CharMetric{  3, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  5, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 10, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 10, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },

	CharMetric{ 14, 0, { 1, 0, 0, 1 }, { 1, 0, 0, 1 } },    // @
	CharMetric{ 11, 0, { 3, 0, 0, 6 }, { 3, 0, 0, 6 }, 0b0000'0110, 0b0110'0000, 15 },
	CharMetric{ 11, 0, { 0, 0, 0, 6 }, { 2, 0, 2, 6 }, 0b0000'1010, 0b0000'0000, 15 },
	CharMetric{ 11, 0, { 1, 0, 1, 6 }, { 1, 0, 1, 6 } },    // C
	CharMetric{ 11, 0, { 0, 0, 0, 6 }, { 0, 0, 0, 6 }, 0b0000'1010, 0b0000'0000, 15 },
	CharMetric{ 11, 0, { 0, 0, 0, 6 }, { 0, 4, 0, 6 }, 0b0000'1010, 0b1010'1010, 12 },
	CharMetric{ 11, 0, { 0, 0, 0, 6 }, { 0, 4, 6, 6 }, 0b0000'1010, 0b1000'1000, 10 },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // G
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  7, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 0b0000'1010, 0b1010'0000, 15 },    // I
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // L
	CharMetric{ 13, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // M
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // N
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 4, 4, 5 }, { 0, 4, 4, 5 }, 0b1000'1000, 0b1000'1000, 10 },    // T
	CharMetric{ 11, 0, { 0, 0, 0, 5 }, { 0, 0, 0, 5 }, 0b0000'1000, 0b1000'0000, 15 },
	CharMetric{ 11, 0, { 0, 0, 0, 5 }, { 0, 0, 0, 5 }, 0b0000'1000, 0b1000'0000, 15 },
	CharMetric{ 13, 0, { 0, 0, 0, 5 }, { 0, 0, 0, 5 }, 0b0000'1010, 0b1010'0000, 15 },    // W
	CharMetric{ 11, 0, { 0, 2, 0, 5 }, { 0, 2, 0, 5 }, 0b0000'1010, 0b1010'0000, 15 },
	CharMetric{ 11, 0, { 0, 0, 0, 5 }, { 0, 0, 0, 5 }, 0b0000'1000, 0b1000'0000, 15 },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 0b1000'1010, 0b1010'0010, 10 },
	CharMetric{  5, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // [
	CharMetric{ 13, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  5, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 11, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },

	CharMetric{  5, 0, { 0, 0, 0, 0 }, { 1, 0, 0, 1 } },    // `
	CharMetric{ 11, 0, { 5, 1, 0, 6 }, { 6, 0, 0, 6 }, 0b0100'0100, 0b0010'0000, 6 },  // a
	CharMetric{ 11, 0, { 0, 0, 0, 6 }, { 8, 0, 0, 6 } },
	CharMetric{  9, 0, { 4, 0, 0, 5 }, { 4, 0, 0, 5 }, 0b0000'0000, 0b0110'0110, 5 },  // c
	CharMetric{ 11, 0, { 8, 0, 0, 6 }, { 0, 0, 0, 6 } },
	CharMetric{ 11, 0, { 0, 0, 0, 6 }, { 0, 0, 0, 6 } },
	CharMetric{  9, 0, { 2, 0, 2, 5 }, { 0, 0, 4, 5 }, 0b0100'0100, 0b1100'1100, 3 },  // f
	CharMetric{ 11, 2, { 6, 0, 0, 0 }, { 6, 0, 0, 1 } },    // g
	CharMetric{ 11, 0, { 0, 0, 0, 6 }, { 8, 0, 0, 6 }, 0b0000'1110, 0b0010'0000, 6 },  // h
	CharMetric{  7, 0, { 2, 0, 0, 4 }, { 2, 2, 0, 4 }, 0b0000'0110, 0b0010'0000, 3 },  // i
	CharMetric{  9, 2, { 6, 6, 5, 0 }, { 0, 0, 0, 0 } },    // j
	CharMetric{ 10, 0, { 0, 0, 0, 5 }, { 7, 0, 0, 5 } },
	CharMetric{  7, 0, { 0, 0, 0, 4 }, { 0, 0, 0, 4 } },    // l
	CharMetric{ 13, 0, { 7, 0, 0, 7 }, { 7, 0, 0, 7 } },    // m
	CharMetric{ 11, 0, { 6, 0, 0, 6 }, { 6, 0, 0, 6 } },    // n
	CharMetric{ 11, 0, { 6, 0, 1, 6 }, { 6, 0, 1, 6 } },
	CharMetric{ 11, 2, { 6, 0, 0, 0 }, { 6, 0, 0, 0 } },    // p
	CharMetric{ 11, 2, { 6, 0, 0, 0 }, { 6, 0, 0, 0 } },    // q
	CharMetric{ 11, 0, { 6, 0, 0, 6 }, { 6, 0, 0, 6 } },
	CharMetric{ 11, 0, { 6, 0, 0, 6 }, { 6, 0, 0, 6 }, 0b0010'0010, 0b0100'0100, 5 },
	CharMetric{ 10, 0, { 4, 0, 4, 5 }, { 4, 1, 0, 5 }, 0b0100'0100, 0b0110'0110, 4 },
	CharMetric{ 11, 0, { 6, 0, 1, 6 }, { 6, 0, 0, 0 } },
	CharMetric{ 11, 0, { 6, 0, 2, 6 }, { 6, 0, 2, 6 } },
	CharMetric{ 13, 0, { 7, 0, 2, 7 }, { 7, 0, 2, 7 } },    // w
	CharMetric{ 11, 0, { 6, 0, 0, 6 }, { 6, 0, 0, 6 } },
	CharMetric{ 11, 2, { 6, 0, 1, 0 }, { 6, 0, 0, 1 } },    // y
	CharMetric{ 11, 0, { 6, 0, 0, 6 }, { 6, 0, 0, 6 }, 0b0100'0100, 0b0010'0010, 4 },
	CharMetric{  9, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },    // {
	CharMetric{  3, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{  9, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 14, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
	CharMetric{ 99, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

#include <unordered_map>

// differentiate between ligatures and just fixing the look
std::unordered_map<string_view, const i8> special_pairs =
	{
		//{"TU", -2},
		//{"Ty", -3},
		/*
		{"ft", -3},
		{"st", -3},
		{"ct", -3},
		{"ff", -2},
		{"tt", -2},
		{"sf", -2},
		{"fi", -2},
		{"is", -2},
		{"li", -1},
		{"ll", -1},
		{"ei", -1},
		{"ie", -1},
		*/
//		{"wi", -1},
//		{"ic", -1},
//		{"ch", -1},
//		{"ig", -1},
//		{"ht", -1},
//		{"ti", -1},
//		{"si", -1},
//		{"le", -1},
//		{"fa", -1},

//		{"ij", -6},
//		{"nj", -6},
//		{"bj", -6},
	};

/*
 * Fonts can merge at: top mid bot low
 * One or Both must be Pointy to allow the connection
 * A merge means that the score at this spot is 2 better
 */

SharedPtr<Texture> tiles;
//Texture * tiles;

inline void TF_Init( Renderer * renderer )
{
	//SDL_DestroyTexture(tiles);
	//tiles = nullptr;
	if( !tiles )
	{
		SharedPtr<Surface>
		//Surface *
		    tileSurf = IMG_Load( BasePath "asset/graphic/16x16_hsnr64.png" );
		SDL_SetColorKey( tileSurf, true, SDL_MapRGB( tileSurf->format, 178, 185, 212 ) );
		tiles = SDL_CreateTextureFromSurface( renderer, tileSurf );
		//SDL_FreeSurface(tileSurf);
		//tiles = IMG_LoadTexture( render, BasePath "asset/graphic/16x16_hsnr64.png" );
		if( !tiles )
			print( stderr, "IMG_LoadTexture failed: {}\n", IMG_GetError() );
	}
}

int numBytesInUTF8( const char c )
{
	if( (c & 0b10000000) == 0 )               return 1;
	else if( (c & 0b11100000) == 0b11000000 ) return 2;
	else if( (c & 0b11110000) == 0b11100000 ) return 3;
	else if( (c & 0b11111000) == 0b11110000 ) return 4;
	else                                      return -1; // Invalid UTF-8 sequence
}

// the typesetting algo should just output an array of CharChains which can be used regardless of backend
struct CharChain {
	char c;
	u8 width;
	u8 left;
};

inline void TF_Render( Renderer * renderer, const String & text, const Rect & dimension )
{
	constexpr u8 ligatureLevel = 25; // 0 off, 9 only ffi, 25 acst
	constexpr u8 overlap_mul = 4;
	constexpr u8 overlap_div = 5;
	constexpr u8 overlap_max = 5;
	constexpr u8 spacing = 2;
	constexpr Point tilesize = { 16, 16 };

	// Shadow section, please develop features in bright section first
	{
		SDL_SetTextureColorMod( tiles, 0, 0, 0 );

		Rect dst{ dimension.x, dimension.y, tilesize.x, tilesize.y };
		char last_c = '\n';
		int skip = 0;
		for( const char c : text )
		{
			// ++ skip non-ascii for now
			if( skip )
			{
				skip -= 1;
				continue;
			}

			const int howManyBytes = numBytesInUTF8( c );
			if(howManyBytes > 1)
			{
				skip = howManyBytes - 1;
				continue;
			}
			// -- skip non-ascii for now

			const bool isNewLine = (c == '\n');
			if( isNewLine || dst.x > dimension.x + dimension.w )
			{
				dst.x = 32;
				dst.y += 20;
				last_c = '\n';

				if( isNewLine )
					continue;
			}

			auto spec = special_pairs.find( String{ last_c, c } );
			if( spec != special_pairs.end() )
			{
				dst.x += spec->second;
			}

			const CharMetric & met_last   = char_metrics[last_c];
			const CharMetric & met_curr   = char_metrics[c];
			const int ligatureProduct     = met_last.ligature_level * met_curr.ligature_level; // max of 65025 (255 * 255)
			const bool isLigaturePossible = ligatureLevel >= ligatureProduct;
			{
				u8 merge = 0;
				if( isLigaturePossible )
				{
					merge = met_last.merge_r & met_curr.merge_l;
					merge |= (merge >> 4);
				}

				i8 sum[4];
				bool hadALigature = false;
				for( int i = 0; i < 4; ++i )
				{
					i8 ligatureOffset = 0;
					if( isLigaturePossible )
					{
						const i8 mask = 0b1000 >> i;
						const bool canConnect = (merge & mask) != 0;

						ligatureOffset = (canConnect && !hadALigature) ? 2 : 0;
						hadALigature |= canConnect;
						// (canConnect ? 2 : 0) == canConnect * 2
					}
					sum[i] = met_last.right[i] + met_curr.left[i] + ligatureOffset;
				}

				const auto min_e = *std::min_element( std::begin( sum ), std::end( sum ) );
				dst.x -= min_e;
			}

			const int x = c % 32;
			const int y = c / 32;
			const Point dst_off = (31 < c && c < 127) ? Point{ 0, met_curr.yoff } : Point{ 0, 0 };
			const Rect src = Rect{ x * 16, y * 16, 16, 16 };
			const Rect dst_final = dst + dst_off;
			for( const Point & pd : shadowOffsets )
			{
				const Rect dst_rect = dst_final + pd;
				SDL_RenderCopy( renderer, tiles, &src, &dst_rect );
			}
			u8 width = met_curr.width;
			dst.x += (31 < c && c < 127) ? width + spacing : 16;

			last_c = c;

			//volatile i8 asd = min_e;
		}
	}

	// Bright section
	{
		SDL_SetTextureColorMod( tiles, 255, 255, 255 );

		Rect dst{ dimension.x, dimension.y, tilesize.x, tilesize.y };
		char last_c = '\n';
		int skip = 0;
		for( const char c : text )
		{
			// ++ skip non-ascii for now
			if( skip )
			{
				skip -= 1;
				continue;
			}

			const int howManyBytes = numBytesInUTF8( c );
			if( howManyBytes > 1 )
			{
				skip = howManyBytes - 1;
				continue;
			}
			// -- skip non-ascii for now

			const bool isNewLine = (c == '\n');
			if( isNewLine || dst.x > dimension.x + dimension.w )
			{
				dst.x = 32;
				dst.y += 20;
				last_c = '\n';

				if( isNewLine )
					continue;
			}

			auto spec = special_pairs.find( String{ last_c, c } );
			if( spec != special_pairs.end() )
			{
				dst.x += spec->second;
			}

			const CharMetric & met_last   = char_metrics[last_c];
			const CharMetric & met_curr   = char_metrics[c];
			const int ligatureProduct     = met_last.ligature_level * met_curr.ligature_level; // max of 65025 (255 * 255)
			const bool isLigaturePossible = ligatureLevel >= ligatureProduct;
			{
				u8 merge = 0;
				if( isLigaturePossible )
				{
					merge = met_last.merge_r & met_curr.merge_l;
					merge |= (merge >> 4);
				}

				i8 sum[4];
				bool hadALigature = false;
				for( int i = 0; i < 4; ++i )
				{
					i8 ligatureOffset = 0;
					if( isLigaturePossible )
					{
						const i8   mask       = 0b1000 >> i;
						const bool canConnect = (merge & mask) != 0;

//								IfDebug if( canConnect && hadALigature )
//										print_once( "Would be double ligature between {} and {}\n", last_c, c );

						ligatureOffset = (canConnect && !hadALigature) ? 2 : 0;
						hadALigature   |= canConnect;
						// (canConnect ? 2 : 0) == canConnect * 2
					}
					sum[i] = met_last.right[i] + met_curr.left[i] + ligatureOffset;

//					if( last_c == 'f' && c == 'a' )
//						print( "for {} and {} index {} is {} lo: {}\n", last_c, c, i, sum[i], ligatureOffset );
				}

				const auto min_e = *std::min_element( std::begin( sum ), std::end( sum ) );
//				static int ii = 0;
//						if(ii++ < 300)
//							print( "for {} and {} moving over {} to the left\n", last_c, c, min_e );
				dst.x -= min_e;
			}

			constexpr int stride = 32;
			const Point index { c % stride, c / stride };
			const Point dst_off = (31 < c && c < 127) ? Point{ 0, met_curr.yoff } : Point{ 0, 0 };
			const Rect src = index * 16 + Rect{ 0, 0, tilesize.x, tilesize.y };
			const Rect dst_final = dst + dst_off;

			SDL_RenderCopy( renderer, tiles, &src, &dst_final );

			u8 width = met_curr.width;
			if(width > 16)
			{
				print( "blah" );
			}
			dst.x += (31 < c && c < 127) ? width + spacing : 16;

			last_c = c;
		}
	}

	if( 0 )
	{
		/* early version, keep for now
		Rect dst{ 32, 100, 16, 16 };
		char last_c = ' ';
		for( const char c : string( text ) )
		{
			if( c == '\n' || dst.x > winSize.x - 30 )
			{
				dst.x = 32;
				dst.y += 20;
				last_c = '\n';
				continue;
			}

			auto spec = special_pairs.find( string{ last_c, c } );
			if( spec != special_pairs.end() )
			{
				dst.x += spec->second;
			}

			const int x = c % 32;
			const int y = c / 32;
			const Point dst_off = (31 < c && c < 127) ? Point{ 0, char_metrics[c].yoff } : Point{ 0, 0 };
			const Rect src = (textmode == 1)
			                 ? Rect{ x * 8, y * 8, 8, 8 }
			                 : Rect{ x * 16, (y + 3) * 16, 16, 16 };
			const Rect dst_final = dst + dst_off;
			SDL_SetTextureColorMod( tiles, 0, 0, 0 );
			for( const Point & pd : shadowOffsets )
			{
				const Rect dst_rect = dst_final + pd;
				SDL_RenderCopy( renderer, tiles, &src, &dst_rect );
			}
			SDL_SetTextureColorMod( tiles, 255, 255, 255 );
			SDL_RenderCopy( renderer, tiles, &src, &dst_final );
			u8 width = char_metrics[c].width;
			if( textmode == 1 )
				width = ((width + 1) / 2) * 2; // approximately
			dst.x += (31 < c && c < 127) ? width + 2 : 16;

			last_c = c;
		}*/
	}
}