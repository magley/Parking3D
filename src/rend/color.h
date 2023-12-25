#pragma once

/// <summary>
/// Color stores a 4-tuple for RGBA.
/// Values are in the range [0.0 to 1.1].
/// </summary>
struct Color {
	float r;
	float g;
	float b;
	float a;
	/// <summary>
	/// RGBA(0, 0, 0, 1).
	/// </summary>
	Color() : r(0), g(0), b(0), a(1) {}
	/// <summary>
	/// RGBA(r, g, b, a).
	/// </summary>
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	/// <summary>
	/// RGBA(r, g, b, 1).
	/// </summary>
	Color(float r, float g, float b) : r(r), g(g), b(b), a(1) {}
};