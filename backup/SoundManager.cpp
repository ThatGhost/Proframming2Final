#include "pch.h"
#include "SoundManager.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <fstream>
#include "dirent.h"

SoundManager::SoundManager()
{
	//m_AllSounds
	struct dirent* entry;
	std::string ObjectAssetPath{ "resources/sound/" };

	DIR* dir = opendir(ObjectAssetPath.c_str());

	if (dir == NULL) {
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		SoundEffect* t = new SoundEffect{ ObjectAssetPath + entry->d_name };
		m_AllSounds.insert(std::pair<std::string, SoundEffect*>(entry->d_name, t));
	}
	closedir(dir);
}

SoundManager::~SoundManager()
{
	for (std::map<std::string, SoundEffect*>::iterator itr = m_AllSounds.begin(); itr != m_AllSounds.end(); itr++)
	{
		delete itr->second;
	}
}

void SoundManager::PlaySound(const std::string& soundName, const int& volume)
{
	m_AllSounds[soundName]->SetVolume(volume);
	m_AllSounds[soundName]->Play(0);
}