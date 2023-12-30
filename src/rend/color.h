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

struct Color_RGB;
struct Color_HSV;

struct Color_RGB {
	double r;       // [0.0, 1.0]
    double g;       // [0.0, 1.0]
    double b;       // [0.0, 1.0]

    Color_HSV to_hsv();
    Color to_color();

	Color_RGB(): r(0), g(0), b(0) {}
	Color_RGB(double r, double g, double b) : r(r), g(g), b(b) {}
};

struct Color_HSV {
    double h;       // [0, 360]
    double s;       // [0.0, 1.0]
    double v;       // [0.0, 1.0]

    Color_RGB to_rgb();
    Color to_color();

	Color_HSV(): h(0), s(0), v(0) {}
	Color_HSV(double h, double s, double v) : h(h), s(s), v(v) {}
};