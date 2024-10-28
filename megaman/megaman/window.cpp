#include "window.h"

//#1 - Constructors and destructor

MainWindow::MainWindow() { Setup("Window", sf::Vector2u(1600, 900)); }

MainWindow::MainWindow(const std::string& l_title, const sf::Vector2u& l_size){
	Setup(l_title, l_size);
}
MainWindow::~MainWindow() { Destroy(); }

//helper function setup

void MainWindow::Setup(const std::string& l_title, const sf::Vector2u& l_size) {
	m_window.setFramerateLimit(60);

	m_windowTitle = l_title;
	m_windowSize = l_size;
	m_isFullscreen = false;
	m_isDone = false;
	Create();
}

//helper functions create and destroy

void MainWindow::Create() {
	auto style = (m_isFullscreen ? sf::Style::Fullscreen: sf::Style::Default);
	m_window.create({ m_windowSize.x, m_windowSize.y, 32 }, m_windowTitle, style); //32 has sth to do with u32int
}
void MainWindow::Destroy() {
	m_window.close();
}

//#2 - Update function

void MainWindow::HandleInput(Event& evt) {
	if (evt.type == sf::Event::Closed) {
		m_isDone = true;
	}
	else if (evt.type == sf::Event::KeyPressed &&
		evt.key.code == sf::Keyboard::F5)
	{
		ToggleFullscreen();
	}

}

//#3 - Toggle Fullscreen function

void MainWindow::ToggleFullscreen() {
	m_isFullscreen = !m_isFullscreen;
	Destroy(); //gotta destroy then re-draw
	Create();
}

//#4,5 - BeginDraw (clear) and EndDraw (display)

void MainWindow::BeginDraw() { m_window.clear(sf::Color::White); }
void MainWindow::EndDraw() { m_window.display(); }

//#6 - Helper methods

bool MainWindow::IsDone() { return m_isDone; }

bool MainWindow::IsFullscreen() { return m_isFullscreen; }

sf::Vector2u MainWindow::GetWindowSize() { return m_windowSize; }

void MainWindow::Draw(sf::Drawable& l_drawable) {
	m_window.draw(l_drawable);
}

RenderWindow* MainWindow::GetRenderWindow() {
	auto ptr = &m_window;
	return ptr;
}