#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "gamestate.hpp"

class IntroState : public IGameState
{
public:
	void Init(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static IntroState* Instance() {
		return &m_IntroState;
	}

protected:
	IntroState() { }

private:
    const sf::Input *mInput;
	static IntroState m_IntroState;
	sf::String m_welcome_message;
};

#endif
