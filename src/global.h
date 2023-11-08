#pragma once

#define _GLIBC_USE_DEPRECATED 0
#define _GLIBCXX_USE_DEPRECATED 0
#define _LIBCPP_ENABLE_DEPRECATION_WARNINGS 2
#define WIN32_LEAN_AND_MEAN

#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cassert>

#include <algorithm>
#include <array>
#include <chrono>
//#include <iostream>
#include <memory>
//#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#if defined( __cpp_lib_format ) && defined( __cpp_lib_print )
#include <format>
#include <print>
#else
#include <fmt/core.h>   // https://fmt.dev/latest/index.html
#endif

#include <nfd.h>        // https://github.com/mlabbe/nativefiledialog/blob/master/README.md

// Usually ImGui is only included during development, but defining IMGUI_ALSO_IN_RELEASE can make it available everywhere
#if defined( _DEBUG ) || (defined( IMGUI_ALSO_IN_RELEASE ) && defined( RELEASE ))
#include <imgui.h>      // https://github.com/ocornut/imgui/blob/bb224c8aa1de1992c6ea3483df56fb04d6d1b5b6/examples/example_sdl2_sdlrenderer/main.cpp
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
//#include <imgui_impl_opengl3.h>
#endif

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

using i8    = std::int8_t;
using i16   = std::int16_t;
using i32   = std::int32_t;
using i64   = std::int64_t;
using u8    = std::uint8_t;
using u16   = std::uint16_t;
using u32   = std::uint32_t;
using u64   = std::uint64_t;
using uint  = unsigned int;

using isize = std::ptrdiff_t;
using usize = std::size_t;

using string      = std::string;
using string_view = std::string_view;
using String      = std::string;
using StringView  = std::string_view;

using Color       = SDL_Color;
using Event       = SDL_Event;
using Point       = SDL_Point;
using FPoint      = SDL_FPoint;
using Keysym      = SDL_Keysym;
using Keymod      = SDL_Keymod;
using Rect        = SDL_Rect;
using FRect       = SDL_FRect;
using Surface     = SDL_Surface;
using Texture     = SDL_Texture;
using Renderer    = SDL_Renderer;
using Window      = SDL_Window;
using Palette     = SDL_Palette;
using PixelFormat = SDL_PixelFormat;
using Font        = TTF_Font;
using Chunk       = Mix_Chunk;
//using Sound       = Mix_Chunk;
using Music       = Mix_Music;

template<class T, std::size_t Size> using Array  = std::array<T, Size>;
template<class T>                   using Vector = std::vector<T>;


template <typename T> class autocast_shared_ptr;

template<class T, class D = std::default_delete<T>> using UniquePtr = std::unique_ptr<T,D>;
template<class T>                                   using SharedPtr = autocast_shared_ptr<T>;
template<class T>                                   using WeakPtr   = std::weak_ptr<T>;

#include <autocast_ptr.h>


using Clock     = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration  = Clock::duration;

using std::min;
using std::max;

//using std::cout, std::cin, std::cerr, std::endl;
#if defined( __cpp_lib_format ) && defined( __cpp_lib_print )
using std::print, std::println, std::format;
#else
using fmt::print, fmt::println, fmt::format;
#endif

// _MSC_VER is also true for Clang on Windows, which is fine, but we might need a branch for CLion vs Visual Studio
#ifdef _MSC_VER
//#define BasePath "../../../"    // out/build/${architecture}-${configuration}
#else
//#define BasePath "../"          // cmake-build-${configuration}
#endif

#ifdef FINAL
#define BasePath ""             // Put the binaries in the root folder, parallel to assets/
#else
#define BasePath "../../../../"    //    out/build/${architecture}-${configuration}
                                // OR build/${configuration}-${compiler}/bin
#endif

#if defined( _DEBUG )
#define DebugOnly( ... ) __VA_ARGS__
#define IfDebug if constexpr( true )
#else
#define DebugOnly( ... )
#define IfDebug if constexpr( false )
#endif

template <typename... T>
inline void print_once(fmt::format_string<T...> fmt, T&&... args)
{
	static std::unordered_set<std::string> all;
	std::string msg = fmt::format( fmt, std::forward<T>( args )... );
	if( all.find( msg ) == all.end() )
	{
		all.insert( msg );
		fmt::print( "{}", msg );
	}
}


#ifdef IMGUI_VERSION

#define IMGUI
#define ImGuiOnly( ... ) __VA_ARGS__

inline void SDL_Init_ImGui()
{
	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, 0 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
#endif

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint( SDL_HINT_IME_SHOW_UI, "1" );
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1  );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   24 );
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8  );
}

inline void SDL_Create_ImGui( Renderer * renderer, Window * window )
{
	//SDL_GLContext gl_context = SDL_GL_CreateContext( window );
	//SDL_GL_MakeCurrent( window, gl_context );
	//SDL_GL_SetSwapInterval( 1 ); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO & io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer( window, renderer );
	ImGui_ImplSDLRenderer2_Init( renderer );

	io.Fonts->AddFontDefault();
	/*
	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	const char * glsl_version = "#version 100";
#elif defined(__APPLE__)
	const char * glsl_version = "#version 150";
#else
	const char * glsl_version = "#version 130";
#endif
	// Setup Platform/Renderer backends
	SDL_GLContext gl_context = SDL_GL_GetCurrentContext();
	ImGui_ImplSDL2_InitForOpenGL( window, gl_context );
	ImGui_ImplOpenGL3_Init( glsl_version );
	*/
}

inline void SDL_ImGui_Frame( Renderer * renderer, Window * window )
{
	static bool show_demo_window = true;

	//ImGuiIO & io = ImGui::GetIO();

	// Start the Dear ImGui frame
	//ImGui_ImplOpenGL3_NewFrame();


	//ImGui::SetNextWindowCollapsed( true, ImGuiCond_Once );
	//ImGui::ShowDemoWindow( &show_demo_window );

	if(0){
		ImGuiIO& io = ImGui::GetIO();

		static float  f           = 0.0f;
		static int    counter     = 0;
		static bool   show_another_window;
		static ImVec4 clear_color = { 0.45f, 0.55f, 0.60f, 1.00f };
		static FRect  col         = { 0.45f, 0.55f, 0.60f, 1.00f };

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//ImGui::
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		ImGui::ColorEdit3("frect color", (float*)&col);         // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	/*
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
	SDL_GL_SwapWindow( window );
	*/
}

#else
#define ImGuiOnly( ... ) // nothing
#endif
