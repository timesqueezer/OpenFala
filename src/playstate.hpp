#ifndef PlayState_H
#define PlayState_H

#include "gamestate.hpp"

class PlayState : public IGameState
{
public:
	void Init(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static PlayState* Instance() {
		return &m_PlayState;
	}

protected:
	PlayState() { }

private:
    const sf::Input *mInput;
	static PlayState m_PlayState;
};

#endif

