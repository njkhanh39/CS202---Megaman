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
		static SoundManager instance; //only initialized one
		return instance;
	}

	void LoadSound(const std::string& name, const std::string& filepath);
	void LoadMusic(const std::string& name, const std::string& filepath);

	void PlaySound(const std::string& name, bool loop);
	void PlayMusic(const std::string& name, bool loop);

	void StopSound(const std::string& name);
	void StopSound();
	void StopMusic(const std::string& name);

	void PauseSound(const std::string& name);
	void PauseSound();

	void SetSoundVolume(const std::string& name, int v) {
		sounds[name]->setVolume(v);
	}

	~SoundManager() {
		for (auto& x : soundBuffers) delete x.second;
		for (auto& x : sounds) delete x.second;
		for (auto& x : music) delete x.second;

		std::cout << "Sound manager destructor.\n";
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
