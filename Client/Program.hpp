#ifndef PROGRAM_H
#define PROGRAM_H

#include "Player.hpp"
#include "Game.hpp"
#include "PauseMenu.hpp"
#include "Engine/GUI/GUIFont.hpp"
#include "Constants.hpp"


class Program {
public:

	void run(sf::IpAddress& ip);


private:

	void initSystems(sf::IpAddress& ip);
	void gameloop();
	void cleanUp();

	Player m_player;
	GUIFont m_font;
	Game m_game;
	PauseMenu m_pause;
	GameStates m_state = GameStates::PLAY;
	sf::Clock m_clock;

};

#endif