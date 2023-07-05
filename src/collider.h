#pragma once

#include <global.h>
#include <recthelper.h>

enum class ColliderType
{
	None,
	All,                // Only for Tiles, AABB the whole Tile
	Point,
	FPoint,
	AABB,               // Axis Aligned Bounding Box
	FAABB,              // Floating-point Axis Aligned Bounding Box
	Circle,
	CenteredCircle,     // Only for Tiles, Circle at center of Tile
	// more complex
	CapsuleVertical,    // AABB + halfcircle caps, higher than wide, the default capsule
	CapsuleHorizontal,  // wider than high
	OBB,                // Oriented Bounding Box, convex
	// even more complex
	Ellipse,
	// etc...
	Composite,          // Contains multiple sub-Colliders
};

union Collider
{
	ColliderType type;
	Point       point;
	FPoint      fpoint;
	Rect        aabb;
	FRect       faabb;
	// structs for the different types
};
