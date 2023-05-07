#pragma once

#include <cmath>
#include <cstdarg>
#include <cstddef>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <SDL_stdinc.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using uint  = unsigned int;
using u8    = std::uint8_t;
using i32   = std::int32_t;
using u32   = std::uint32_t;

using Color     = SDL_Color;
using Event     = SDL_Event;
using Point     = SDL_Point;
using FPoint    = SDL_FPoint;
using Keysym    = SDL_Keysym;
using Rect      = SDL_Rect;
using FRect     = SDL_FRect;
using Surface   = SDL_Surface;
using Texture   = SDL_Texture;
using Renderer  = SDL_Renderer;
using Window    = SDL_Window;
using Font      = TTF_Font;
using Music     = Mix_Music;

using std::min;
using std::max;

template<class T, size_t Size>  using Array  = std::array<T, Size>;
template<class T>               using Vector = std::vector<T>;

using Clock     = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration  = Clock::duration;

using std::cout, std::cin, std::cerr, std::endl;

#ifdef _MSC_VER
#define BasePath ""
#else
#define BasePath "../"
#endif

#if defined( _DEBUG )
#define DebugOnly( ... ) __VA_ARGS__
#define IfDebug if constexpr( true )
#else
#define DebugOnly( ... )
#define IfDebug if constexpr( false )
#endif
