#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdexcept>

class SoundManager {
public:
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	void LoadSound(const std::string& name, const std::string& filepath);
	void LoadMusic(const std::string& name, const std::string& filepath);

	void PlaySound(const std::string& name, bool loop);
	void PlayMusic(const std::string& name, bool loop);
	void StopMusic();

	~SoundManager() {
		for (auto& x : soundBuffers) delete x.second;
		for (auto& x : sounds) delete x.second;
		for (auto& x : music) delete x.second;
	}
private:
	std::map<std::string, sf::SoundBuffer*> soundBuffers;
	std::map <std::string, sf::Sound*> sounds;
	std::map<std::string, sf::Music*> music;

	//-----------------
	SoundManager(){}
	SoundManager(const SoundManager&){}
	SoundManager& operator=(const SoundManager&){}
};

#endif 
