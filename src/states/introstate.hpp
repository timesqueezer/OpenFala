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

	void HandleEvents();
	void Update();
	void Draw();

	static IntroState* Instance() {
		return &m_IntroState;
	}

protected:
	IntroState() { }

private:
	static IntroState m_IntroState;
	sf::String m_welcome_message;
};

#endif
