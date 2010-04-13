#ifndef GAMEENGINE_CPP_INCLUDED
#define GAMEENGINE_CPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

#include "utility.hpp"
#include "resources.hpp"
#include "gamestate.hpp"

//class IGameState;

class GameEngine{

public:
    GameEngine(const po::variables_map& vm);

	void Init(std::string title, sf::Uint16 width=800, sf::Uint16 height=600, bool fullscreen=false);
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

	sf::Uint16 m_width;
	sf::Uint16 m_height;

private:
	// the stack of states

	// TODO: boost.ptr_vector
	std::vector<IGameState*> states;

	bool m_running;
	bool m_fullscreen;

};

#endif
