#pragma once
#include "Scene.h"
#include <vector>

class Game
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update(float elapsedSec);
	void Draw( ) const;
	void ClearBackground() const;
	void SetCurrentSceneIdx(const int& idx) { m_CurrentSceneIdx = idx; }

	bool IsGameRunning() { return m_GameRunning; }
	void QuitGame() { m_GameRunning = false; };
	void GoBackToMain();
private:
	const Window m_Window;
	
	int m_CurrentSceneIdx{};

	bool m_GameRunning{ true };
	std::vector<Scene*> m_Scenes{};

	void Initialize( );
	void Cleanup( );
};