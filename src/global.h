#pragma once

#include <array>
#include <algorithm>

#include <cstdarg>
#include <cstddef>
#include <cmath>

#include <algorithm>
#include <chrono>
#include <memory>
#include <sstream>
#include <iostream>
#include <string>

#include <SDL_stdinc.h>

#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <SDL_ttf.h>

#include <SDL_image.h>

using uint  = unsigned int;
using i32   = int32_t;
using u32   = uint32_t;

using Color     = SDL_Color;
using Event     = SDL_Event;
using Point     = SDL_Point;
using Keysym    = SDL_Keysym;
using Rect      = SDL_Rect;
using Surface   = SDL_Surface;
using Texture   = SDL_Texture;
using Renderer  = SDL_Renderer;
using Window    = SDL_Window;
using Font      = TTF_Font;

constexpr const Rect * EntireRect = nullptr;

using std::min;
using std::max;

template<class T, size_t Size>  using Array  = std::array<T, Size>;
template<class T>               using Vector = std::vector<T>;

using Clock     = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration  = Clock::duration;

using std::cout, std::cin, std::cerr, std::endl;
