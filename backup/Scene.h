#pragma once

class Scene
{
public:
	Scene(int idx) : m_SceneIndex{ idx }
	{}
	virtual ~Scene() {};
	virtual void Start(const Window& window) {};
	virtual void Draw() const {};
	virtual void Update(const float& elapsedSec) {};

	int GetSceneIndex() const { return m_SceneIndex; }
	
private:
	int m_SceneIndex;
};

