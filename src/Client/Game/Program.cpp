#include "Program.hpp"
#include <iostream>

void Program::run(){
	initSystems();
	gameloop();
	cleanUp();
}

void Program::initSystems(){
	m_config.loadFromFile();
	m_settings.loadFromFile();
	Window::create(m_config.getWindowWidth(), m_config.getWindowHeight(), "TheTowers");
	InputManager::init(Window::getWindowPtr());
	InputManager::setVerticalSync(true);
	InputManager::setMouseGrabbed(true);
	GUIRenderer::init(m_config.getWindowWidth(), m_config.getWindowHeight(), GUIAssets::getTexture("gui_sprite_sheet"));
	GUIUVLoader::init();
	// m_networkManager.connectToServer(DEFAULT_IP, &m_config);
	m_game.init(&m_networkManager, &m_config, &m_settings, &m_state);
	m_pause.init(&m_settings);
}

void Program::gameloop(){
	m_deltaTimer.restart();
	while(m_state != GameStates::EXIT){
		Window::clear();
		if(InputManager::processInput()) m_state = GameStates::EXIT;
		float deltaTime = m_deltaTimer.getElapsedTime();
		m_deltaTimer.restart();
		m_game.updateEssentials(deltaTime);

		if(m_state == GameStates::PLAY){
			m_game.update(deltaTime);
			m_game.render();
		}else if(m_state == GameStates::PAUSE){
			m_pause.update(m_state, deltaTime);
			m_game.render();
			m_pause.render();
		}
		GUIRenderer::batch();
		GUIRenderer::render();

		Window::update();
	}
}

void Program::cleanUp(){
	GUIRenderer::destroy();
	m_game.destroy();
	Window::close();
}
