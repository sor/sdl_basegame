#pragma once

#include <global.h>

template<typename T>
class autocast_shared_ptr;

#define AUTOCAST_PTR_IMPLICIT_ASSIGN
#define AUTOCAST_PTR_ONLY_FOR_SDL

// Usage: auto blah = Share( tex ); // blah will be of type SharedPtr<Texture> with the correct deleter assigned
template <typename T>
SharedPtr<T> Share( T * p )
#ifdef AUTOCAST_PTR_ONLY_FOR_SDL
= delete;
#else
{ return SharedPtr<T> { p }; }
#endif

//template <typename T>
//UniquePtr<T> Unique( T * p )
//#ifdef AUTOCAST_PTR_ONLY_FOR_SDL
//= delete;
//#else
//{ return UniquePtr<T> { p }; }
//#endif

//template <typename T, typename D = std::default_delete<T>>
//class autocast_unique_ptr : public std::unique_ptr<T,D>
//{
//public:
//	using std::unique_ptr<T,D>::unique_ptr;
//
//	constexpr inline operator       T *()       noexcept { return this->get(); }
//	constexpr inline operator const T *() const noexcept { return this->get(); }
//};

template<typename T>
class autocast_shared_ptr : public std::shared_ptr<T>
{
	template <typename... Args> using Constructible = typename std::enable_if_t<std::is_constructible<std::shared_ptr<T>, Args...>::value>;
	template <typename... Args> using Shareible     = typename std::enable_if_t<std::is_invocable<    decltype(Share<T>), Args...>::value>;

public:
	using std::shared_ptr<T>::shared_ptr;

//	template <typename Y, typename = Constructible<Y*>>
//	inline autocast_shared_ptr( Y * p ) noexcept : std::shared_ptr<T>( p ) { }

//	template <typename Y, typename = Constructible<Y*>>
//	inline autocast_shared_ptr& operator = ( Y * p ) noexcept { this->reset( p ); return *this; }

#ifdef AUTOCAST_PTR_IMPLICIT_ASSIGN
	template <typename Y, typename = Shareible<Y*>>	constexpr inline autocast_shared_ptr             ( Y * p ) noexcept : std::shared_ptr<T>( Share( p ) ) { }
//	template <typename Y, typename = Shareible<Y*>>	constexpr inline autocast_shared_ptr& operator = ( Y * p ) noexcept              { this = Share( p ); return *this; }
#endif

	constexpr inline operator       T *()       noexcept { return this->get(); }
	constexpr inline operator const T *() const noexcept { return this->get(); }
};

// A deleted templated base func with specialized implementations disallows derived types as parameters for those specializations

template<> inline SharedPtr<Surface>  Share( Surface  * p ) { return SharedPtr<Surface> { p, &SDL_FreeSurface       }; }
template<> inline SharedPtr<Texture>  Share( Texture  * p ) { return SharedPtr<Texture> { p, &SDL_DestroyTexture    }; }
template<> inline SharedPtr<Renderer> Share( Renderer * p ) { return SharedPtr<Renderer>{ p, &SDL_DestroyRenderer   }; }
template<> inline SharedPtr<Window>   Share( Window   * p ) { return SharedPtr<Window>  { p, &SDL_DestroyWindow     }; }
template<> inline SharedPtr<Font>     Share( Font     * p ) { return SharedPtr<Font>    { p, &TTF_CloseFont         }; }
template<> inline SharedPtr<Chunk>    Share( Chunk    * p ) { return SharedPtr<Chunk>   { p, &Mix_FreeChunk         }; }
template<> inline SharedPtr<Music>    Share( Music    * p ) { return SharedPtr<Music>   { p, &Mix_FreeMusic         }; }

//inline void blahblub( Renderer * r, int index, Uint32 flags )
//{
//	autocast_shared_ptr<Renderer> x = r;
//	volatile std::shared_ptr y = x;
//	SDL_RenderDrawPoint( r, 9, 9 );
//}
