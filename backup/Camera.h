#pragma once
#include "Vector2f.h"

class Camera
{
public:
	static Camera& Get() {
		static Camera s_Camera{2};
		return s_Camera;
	}
	float GetZoom() const { return m_Zoom; }
	Vector2f GetPos() const { return m_CamPos; }
	void SetPos(const Vector2f NewPos) { m_CamPos = NewPos; }
private:
	Camera(const float& zoom);
	Vector2f m_CamPos;
	float m_Zoom;
};