#pragma once

#include "global.h"

constexpr const SDL_Rect * EntireRect = nullptr;

constexpr SDL_Point   operator"" _up    (unsigned long long n) { return { 0, -(int)n }; }
constexpr SDL_Point   operator"" _down  (unsigned long long n) { return { 0,  (int)n }; }
constexpr SDL_Point   operator"" _left  (unsigned long long n) { return { -(int)n, 0 }; }
constexpr SDL_Point   operator"" _right (unsigned long long n) { return {  (int)n, 0 }; }

constexpr SDL_Point   operator+  (const SDL_Point lhs, const SDL_Point rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
constexpr SDL_Point   operator-  (const SDL_Point lhs, const SDL_Point rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
constexpr SDL_Point   operator*  (const SDL_Point lhs, const SDL_Point rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y }; }
constexpr SDL_Point   operator/  (const SDL_Point lhs, const SDL_Point rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y }; }
constexpr SDL_Point&  operator+= (SDL_Point& lhs, const SDL_Point rhs) { lhs = lhs + rhs; return lhs; }
constexpr SDL_Point&  operator-= (SDL_Point& lhs, const SDL_Point rhs) { lhs = lhs - rhs; return lhs; }

constexpr SDL_Point   operator*  (const SDL_Point lhs, const int rhs) { return { lhs.x * rhs, lhs.y * rhs }; }
constexpr SDL_Point   operator/  (const SDL_Point lhs, const int rhs) { return { lhs.x / rhs, lhs.y / rhs }; }
constexpr SDL_Point&  operator*= (SDL_Point& lhs, const int rhs) { lhs = lhs * rhs; return lhs; }
constexpr SDL_Point&  operator/= (SDL_Point& lhs, const int rhs) { lhs = lhs / rhs; return lhs; }


constexpr SDL_Rect    operator+  (const SDL_Rect lhs, const SDL_Rect rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.h }; }
constexpr SDL_Rect    operator-  (const SDL_Rect lhs, const SDL_Rect rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.w - rhs.w, lhs.h - rhs.h }; }
constexpr SDL_Rect&   operator+= (SDL_Rect& lhs, const SDL_Rect rhs) { lhs = lhs + rhs; return lhs; }
constexpr SDL_Rect&   operator-= (SDL_Rect& lhs, const SDL_Rect rhs) { lhs = lhs - rhs; return lhs; }

constexpr SDL_Rect    operator+  (const SDL_Point lhs, const SDL_Rect rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, rhs.w, rhs.h }; }
constexpr SDL_Rect    operator+  (const SDL_Rect lhs, const SDL_Point rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h }; }
constexpr SDL_Rect    operator-  (const SDL_Rect lhs, const SDL_Point rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.w, lhs.h }; }
constexpr SDL_Rect    operator*  (const SDL_Rect lhs, const SDL_Point rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.w, lhs.h }; }
constexpr SDL_Rect    operator/  (const SDL_Rect lhs, const SDL_Point rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.w, lhs.h }; }
constexpr SDL_Rect&   operator+= (SDL_Rect& lhs, const SDL_Point rhs) { lhs = lhs + rhs; return lhs; }
constexpr SDL_Rect&   operator-= (SDL_Rect& lhs, const SDL_Point rhs) { lhs = lhs - rhs; return lhs; }
constexpr SDL_Rect&   operator*= (SDL_Rect& lhs, const SDL_Point rhs) { lhs = lhs * rhs; return lhs; }
constexpr SDL_Rect&   operator/= (SDL_Rect& lhs, const SDL_Point rhs) { lhs = lhs / rhs; return lhs; }

constexpr SDL_Rect    operator*  (const SDL_Rect lhs, const int rhs) { return { lhs.x, lhs.y, lhs.w * rhs, lhs.h * rhs }; }
constexpr SDL_Rect    operator/  (const SDL_Rect lhs, const int rhs) { return { lhs.x, lhs.y, lhs.w / rhs, lhs.h / rhs }; }
constexpr SDL_Rect&   operator*= (SDL_Rect& lhs, const int rhs) { lhs = lhs * rhs; return lhs; }
constexpr SDL_Rect&   operator/= (SDL_Rect& lhs, const int rhs) { lhs = lhs / rhs; return lhs; }


constexpr SDL_FPoint  operator"" _up    (long double n) { return { 0.0f, -(float)n }; }
constexpr SDL_FPoint  operator"" _down  (long double n) { return { 0.0f,  (float)n }; }
constexpr SDL_FPoint  operator"" _left  (long double n) { return { -(float)n, 0.0f }; }
constexpr SDL_FPoint  operator"" _right (long double n) { return {  (float)n, 0.0f }; }

constexpr SDL_FPoint  operator+  (const SDL_FPoint lhs, const SDL_FPoint rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
constexpr SDL_FPoint  operator-  (const SDL_FPoint lhs, const SDL_FPoint rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
constexpr SDL_FPoint  operator*  (const SDL_FPoint lhs, const SDL_FPoint rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y }; }
constexpr SDL_FPoint  operator/  (const SDL_FPoint lhs, const SDL_FPoint rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y }; }
constexpr SDL_FPoint& operator+= (SDL_FPoint& lhs, const SDL_FPoint rhs) { lhs = lhs + rhs; return lhs; }
constexpr SDL_FPoint& operator-= (SDL_FPoint& lhs, const SDL_FPoint rhs) { lhs = lhs - rhs; return lhs; }

constexpr SDL_FPoint  operator*  (const SDL_FPoint lhs, const float rhs) { return { lhs.x * rhs, lhs.y * rhs }; }
constexpr SDL_FPoint  operator/  (const SDL_FPoint lhs, const float rhs) { return { lhs.x / rhs, lhs.y / rhs }; }
constexpr SDL_FPoint& operator*= (SDL_FPoint& lhs, const float rhs) { lhs = lhs * rhs; return lhs; }
constexpr SDL_FPoint& operator/= (SDL_FPoint& lhs, const float rhs) { lhs = lhs / rhs; return lhs; }


constexpr SDL_FRect    operator+  (const SDL_FRect lhs, const SDL_FRect rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.h }; }
constexpr SDL_FRect    operator-  (const SDL_FRect lhs, const SDL_FRect rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.w - rhs.w, lhs.h - rhs.h }; }
constexpr SDL_FRect&   operator+= (SDL_FRect& lhs, const SDL_FRect rhs) { lhs = lhs + rhs; return lhs; }
constexpr SDL_FRect&   operator-= (SDL_FRect& lhs, const SDL_FRect rhs) { lhs = lhs - rhs; return lhs; }

constexpr SDL_FRect    operator+  (const SDL_FPoint lhs, const SDL_FRect rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, rhs.w, rhs.h }; }
constexpr SDL_FRect    operator+  (const SDL_FRect lhs, const SDL_FPoint rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h }; }
constexpr SDL_FRect    operator-  (const SDL_FRect lhs, const SDL_FPoint rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.w, lhs.h }; }
constexpr SDL_FRect&   operator+= (SDL_FRect& lhs, const SDL_FPoint rhs) { lhs = lhs + rhs; return lhs; }
constexpr SDL_FRect&   operator-= (SDL_FRect& lhs, const SDL_FPoint rhs) { lhs = lhs - rhs; return lhs; }

constexpr SDL_FRect    operator*  (const SDL_FRect lhs, const float rhs) { return { lhs.x, lhs.y, lhs.w * rhs, lhs.h * rhs }; }
constexpr SDL_FRect    operator/  (const SDL_FRect lhs, const float rhs) { return { lhs.x, lhs.y, lhs.w / rhs, lhs.h / rhs }; }
constexpr SDL_FRect&   operator*= (SDL_FRect& lhs, const float rhs) { lhs = lhs * rhs; return lhs; }
constexpr SDL_FRect&   operator/= (SDL_FRect& lhs, const float rhs) { lhs = lhs / rhs; return lhs; }

