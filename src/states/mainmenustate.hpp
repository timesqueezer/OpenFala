# ifndef MAINMENUSTATE_HPP_INCLUDED
# define MAINMENUSTATE_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include "gamestate.hpp"
#include "cpGUI.hpp"

class MainMenuState : public IGameState
{
public:
	void Init(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

	static MainMenuState * Instance() {
		return &m_MainMenuState;
	}

protected:
	MainMenuState() { }

private:
	static MainMenuState m_MainMenuState;

	cp::cpGuiContainer gui;
	cp::cpButton *gameStartButton;
	cp::cpButton *optionsButton;
	cp::cpButton *exitButton;

	sf::Music m_MenuMusic;
};

#endif
