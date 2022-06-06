#include "pch.h"
#include "structs.h"

//-----------------------------------------------------------------
// Window Constructors
//-----------------------------------------------------------------
Window::Window( const std::string& title , float width , float height , bool isVSyncOn )
	:title{ title }
	,width{ width }
	,height{ height }
	,isVSyncOn{ isVSyncOn }
{
}

//-----------------------------------------------------------------
// Point2f Constructors
//-----------------------------------------------------------------
Point2f::Point2f( )
	:Point2f{ 0.0f, 0.0f }
{
}
Point2f::Point2f( float x, float y )
	:x{ x }, y{ y }
{
}

Point2f* Point2f::operator+=(const Point2f& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	return this;
}

Point2f operator+(Point2f lhs, const Point2f& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

std::ostream& operator<<(std::ostream& os,const Point2f& rhs)
{
	os << "( " << rhs.x << ", " << rhs.y << ")";
	return os;
}
//-----------------------------------------------------------------
// Point2i Constructors
//-----------------------------------------------------------------
Point2i::Point2i()
	:Point2i{ 0, 0 }
{
}
Point2i::Point2i(int x, int y)
	: x{ x }, y{ y }
{
}

Point2i* Point2i::operator+=(const Point2i& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	return this;
}

Point2i operator+(Point2i lhs, const Point2i& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

std::ostream& operator<<(std::ostream& os, const Point2i& rhs)
{
	os << "( " << rhs.x << ", " << rhs.y << ")";
	return os;
}


bool operator==(Point2i lhs, Point2i rhs) 
{
	return lhs.y == rhs.y && lhs.x == rhs.x;
}

//-----------------------------------------------------------------
// InputS Constructors
//-----------------------------------------------------------------
InputS::InputS()
	: mousePos{}, ClickDown{ ClickState::NA }, ClickUp{ ClickState::NA },
	ScrollWheel{}
{
}

//-----------------------------------------------------------------
// Rectf Constructors
//-----------------------------------------------------------------
Rectf::Rectf( )
	:Rectf{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

Rectf::Rectf( float left, float bottom, float width, float height )
	:left{ left }
	,bottom{ bottom }
	,width{ width }
	,height{ height }
{
}

//-----------------------------------------------------------------
// Color3 Constructors
//-----------------------------------------------------------------

Color3::Color3()
	:Color3{ 255, 255, 255 }
{}

Color3::Color3(int gr, int gg, int gb) {
	r = gr;
	g = gg;
	b = gb;
}
//-----------------------------------------------------------------
// Color4f Constructors
//-----------------------------------------------------------------
Color4f::Color4f( )
	:Color4f{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

Color4f::Color4f( float r, float g, float b, float a )
	:r{ r }
	,g{ g }
	,b{ b }
	,a{ a }
{
}

//-----------------------------------------------------------------
// Circlef Constructors
//-----------------------------------------------------------------
Circlef::Circlef( )
	:Circlef{ 0.0f, 0.0f, 0.0f }
{
}

Circlef::Circlef( float centerX, float centerY, float radius )
	:Circlef{ Point2f{ centerX, centerY }, radius }
{
}

Circlef::Circlef( const Point2f& center, float radius )
	:center{ center }
	,radius{ radius }
{
}

//-----------------------------------------------------------------
// Ellipsef Constructors
//-----------------------------------------------------------------
Ellipsef::Ellipsef( )
	:Ellipsef{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}


Ellipsef::Ellipsef( const Point2f& center, float radiusX, float radiusY )
	: center{ center }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsef::Ellipsef( float centerX, float centerY, float radiusX, float radiusY )
	: Ellipsef{ Point2f{ centerX, centerY }, radiusX, radiusY }
{
}
