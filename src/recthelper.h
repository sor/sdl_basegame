#pragma once

#include "global.h"

constexpr const Rect * EntireRect = nullptr;

constexpr Point  operator"" _up    (unsigned long long n) { return Point{ 0, -(int)n }; }
constexpr Point  operator"" _down  (unsigned long long n) { return Point{ 0, (int)n }; }
constexpr Point  operator"" _left  (unsigned long long n) { return Point{ -(int)n, 0 }; }
constexpr Point  operator"" _right (unsigned long long n) { return Point{ (int)n, 0 }; }

constexpr Point  operator+  (const Point lhs, const Point rhs) { return Point{ lhs.x + rhs.x, lhs.y + rhs.y }; }
constexpr Point  operator-  (const Point lhs, const Point rhs) { return Point{ lhs.x - rhs.x, lhs.y - rhs.y }; }
constexpr Point& operator+= (Point& lhs, const Point rhs) { lhs = lhs + rhs; return lhs; }
constexpr Point& operator-= (Point& lhs, const Point rhs) { lhs = lhs - rhs; return lhs; }
constexpr Rect   operator+  (const Point lhs, const Rect rhs) { return Rect{ lhs.x + rhs.x, lhs.y + rhs.y, rhs.w, rhs.h }; }
constexpr Rect   operator+  (const Rect lhs, const Point rhs) { return Rect{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h }; }
constexpr Rect   operator-  (const Rect lhs, const Point rhs) { return Rect{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.w, lhs.h }; }
constexpr Rect&  operator+= (Rect& lhs, const Point rhs) { lhs = lhs + rhs; return lhs; }
constexpr Rect&  operator-= (Rect& lhs, const Point rhs) { lhs = lhs - rhs; return lhs; }
constexpr Rect   operator+  (const Rect lhs, const Rect rhs) { return Rect{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.h }; }
constexpr Rect   operator-  (const Rect lhs, const Rect rhs) { return Rect{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.w - rhs.w, lhs.h - rhs.h }; }
constexpr Rect&  operator+= (Rect& lhs, const Rect rhs) { lhs = lhs + rhs; return lhs; }
constexpr Rect&  operator-= (Rect& lhs, const Rect rhs) { lhs = lhs - rhs; return lhs; }
