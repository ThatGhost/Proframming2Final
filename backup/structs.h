#pragma once
#include <string>

struct Window
{
	explicit Window( const std::string& title = "Title", float width = 320.0f, 
		float height = 180.0f, bool isVSyncOn = true );

	std::string title;
	float width;
	float height;
	bool isVSyncOn;
};

struct Point2f
{
	Point2f( );
	explicit Point2f( float x, float y );

	Point2f* operator+=(const Point2f& rhs);
	friend Point2f operator+(Point2f lhs, const Point2f& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Point2f& rhs);
	float x;
	float y;
};

struct Point2i
{
	Point2i();
	explicit Point2i(int x, int y);

	Point2i* operator+=(const Point2i& rhs);
	friend Point2i operator+(Point2i lhs, const Point2i& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Point2i& rhs);

	int x;
	int y;
};

bool operator==(Point2i lhs, Point2i rhs);

enum class ClickState {
	NA,
	MB1,
	MB2,
	MB3
};

struct InputS {
	InputS();
	SDL_Keycode keyDown{};
	SDL_Keycode keyUp{};
	Point2f mousePos;
	ClickState ClickDown;
	ClickState ClickUp;
	float ScrollWheel;
};

struct Rectf
{
	Rectf( );
	explicit Rectf( float left, float bottom, float width, float height );
	
	float left;
	float bottom;
	float width;
	float height;

};

struct Color3
{
	Color3();
	explicit Color3(int r, int g, int b);

	int r;
	int g;
	int b;
};

struct Color4f
{
	Color4f( );
	explicit Color4f( float r, float g, float b, float a );
	
	float r;
	float g;
	float b;
	float a;
};

struct Circlef
{
	Circlef( );
	explicit Circlef( const Point2f& center, float radius );
	explicit Circlef( float centerX, float centerY, float radius );

	Point2f center;
	float radius;
};


struct Ellipsef
{
	Ellipsef( );
	explicit Ellipsef( const Point2f& center, float radiusX, float radiusY );
	explicit Ellipsef( float centerX, float centerY, float radiusX, float radiusY );

	Point2f center;
	float radiusX;
	float radiusY;
};



