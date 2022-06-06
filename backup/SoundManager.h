#pragma once
#include <map>
class SoundEffect;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	void PlaySound(const std::string& soundName, const int& volume = 20);
private:
	std::map<std::string, SoundEffect*> m_AllSounds;
};

