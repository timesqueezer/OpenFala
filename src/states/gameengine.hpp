#ifndef GAMEENGINE_CPP_INCLUDED
#define GAMEENGINE_CPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <string>

#include "../utility/utility.hpp"
#include "../resources/resources.hpp"
#include "gamestate.hpp"
#include "../entity/EntityManager.hpp"

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

	ResourceManager& GetResMgr();
	EntityManager& GetEntMgr();

	void StartMusic(std::string filename);
	void StopMusic();
	bool MusicPlaying();

	sf::RenderWindow app;

	po::variables_map mProgramOptions;

	sf::Uint16 m_width;
	sf::Uint16 m_height;

private:
    sf::Music mBackgroundMusic;
	// TODO: Singleton
	ResourceManager mResMgr;

	// TODO: Singleton
	EntityManager mEntMgr;

    // the stack of states
	// TODO: boost.ptr_vector
	std::vector<IGameState*> states;

	bool m_running;
	bool m_fullscreen;

};

#endif
