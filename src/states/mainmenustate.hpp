# ifndef MAINMENUSTATE_HPP_INCLUDED
# define MAINMENUSTATE_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include "gamestate.hpp"
#include "../gui/cpGUI.hpp"

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

	cp::cpShapeButton *testButton;
	cp::cpImageButton *imgButton;

	sf::Shape testShape;

	sf::Sprite mBackgroundImage;

	sf::Music m_MenuMusic;
};

#endif
