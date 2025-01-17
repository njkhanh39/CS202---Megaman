#include "SoundManager.h"
#include <stdexcept>

void SoundManager::LoadSound(const std::string& name, const std::string& filepath) {
	if (soundBuffers.find(name) != soundBuffers.end()) {
		return;
	}

	soundBuffers[name] = new sf::SoundBuffer();

	if (!soundBuffers[name]->loadFromFile(filepath)) {
		throw std::runtime_error("Failed to load sound: " + filepath);
	}

	sounds[name] = new sf::Sound();
	sounds[name]->setBuffer(*soundBuffers[name]);
}

void SoundManager::LoadMusic(const std::string& name, const std::string& filepath) {
	if (music.find(name) != music.end()) {
		throw std::runtime_error("Music already loaded: " + name);
	}

	if (!music[name]->openFromFile(filepath)) {
		throw std::runtime_error("Failed to load music: " + filepath);
	}
}

void SoundManager::PlaySound(const std::string& name, bool loop) {
	if (sounds.find(name) == sounds.end()) {
		throw std::runtime_error("Sound not found: " + name);
	}

	sounds[name]->play();
	sounds[name]->setLoop(loop);
}

void SoundManager::PlayMusic(const std::string& name, bool loop) {
	if (music.find(name) == music.end()) {
		throw std::runtime_error("Music not found: " + name);
	}

	music[name]->play();
	sounds[name]->setLoop(loop);
}

void SoundManager::StopSound(const std::string& name) {
	if (!sounds[name]) return;
	sounds[name]->stop();
}

void SoundManager::StopMusic(const std::string& name) {
	if (!sounds[name]) return;
	music[name]->stop();
}

void SoundManager::PauseSound(const std::string& name) {
	if (!sounds[name]) return;
	sounds[name]->pause();
}

void SoundManager::PauseSound() {
	for (auto& s : sounds) {
		s.second->pause();
	}
}

void SoundManager::StopSound() {
	for (auto& s : sounds) {
		s.second->stop();
	}
}