#ifndef GAMEENGINE_CPP_INCLUDED
#define GAMEENGINE_CPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include "utility.hpp"
#include "resources.hpp"
#include "gamestate.hpp"

//class IGameState;

class GameEngine{

public:
    GameEngine(const po::variables_map& vm);

	void Init(const char* title, int width=800, int height=600, bool fullscreen=false);
	void Cleanup();

	void ChangeState(IGameState* state);
	void PushState(IGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

	sf::RenderWindow app;
	// TODO: Singleton
	ResourceManager mResMgr;

	po::variables_map mProgramOptions;

	uint16_t m_width;
	uint16_t m_height;

private:
	// the stack of states

	// TODO: boost.ptr_vector
	std::vector<IGameState*> states;

	bool m_running;
	bool m_fullscreen;

};

#endif
