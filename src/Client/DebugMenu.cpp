#include "DebugMenu.hpp"


void DebugMenu::init(Game* _game, GUICanvas* _canvas){
	m_game = _game;
	m_canvas = _canvas;
	initGUI();
}

void DebugMenu::render(){
	updateGUI();
	m_canvas->render();
}

void DebugMenu::initGUI(){
	m_canvas->labels.push_back(GUILabel(std::string("X: "), glm::vec2(25, 675), ColorRGBA8()));
	m_canvas->labels.push_back(GUILabel(std::string("Y: "), glm::vec2(25, 625), ColorRGBA8()));
	m_canvas->labels.push_back(GUILabel(std::string("Z: "), glm::vec2(25, 575), ColorRGBA8()));
}

void DebugMenu::updateGUI(){
	glm::vec3 camPos = m_game->camera.getPosition();
	
	m_canvas->labels[0].setString("X: " + std::to_string(camPos.x));
	m_canvas->labels[1].setString("Y: " + std::to_string(camPos.y));
	m_canvas->labels[2].setString("Z: " + std::to_string(camPos.z));
}