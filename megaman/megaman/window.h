#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class MainWindow {
public:
	//constructors & destructors

	MainWindow();
	MainWindow(const std::string& l_title, const sf::Vector2u& l_size);
	~MainWindow();

	//funcs

	//when setting this, m_done becomes true, main gameloop will stop
	void SetFinish();
	void BeginDraw(); // Clear the window.
	void EndDraw(); // Display the changes.

	//---Important----
	void HandleEvents(Event& evt); 

	//-------------

	bool IsDone(); //window is done = turned off
	bool IsFullscreen();
	sf::Vector2u GetWindowSize();
	void ToggleFullscreen();
	void Draw(sf::Drawable& l_drawable); //Pass in sth to draw

	//getters

	RenderWindow* GetRenderWindow();
private:
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone;
	bool m_isFullscreen;

	//helpers

	void Setup(const std::string& l_title, const sf::Vector2u& l_size);
	void Destroy();
	void Create();
	
};