#include <boost/program_options.hpp>
#include <SFML/System.hpp>

#include <string>

#include "../gui/cpGUI.hpp"
#include "gameengine.hpp"
#include "gamestate.hpp"

GameEngine::GameEngine(const po::variables_map& vm){
    mProgramOptions = vm;
};

void GameEngine::Init(std::string title, sf::Uint16 width, sf::Uint16 height, bool fullscreen)
{
	m_width = width;
	m_height = height;

    // Create Game Window
    if (fullscreen)
        app.Create(sf::VideoMode(m_width, m_height, 32), title, sf::Style::Fullscreen);
    else
        app.Create(sf::VideoMode(m_width, m_height, 32), title, sf::Style::Close);

    app.SetFramerateLimit(60);

	m_fullscreen = fullscreen;
	m_running = true;

	std::cout << "GameEngine Init" << std::endl;
}

void GameEngine::Cleanup()
{
	// Cleanup the all states
	while ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	std::cout << "GameEngine Cleanup" << std::endl;

    m_running = false;

    // Close Window if it is still open
    if (app.IsOpened())
        app.Close();
}

void GameEngine::ChangeState(IGameState* state)
{
	// Cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// Store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void GameEngine::PushState(IGameState* state)
{
	// Pause current state
	if ( !states.empty() ) {
		states.back()->Pause();
	}

	// Store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void GameEngine::PopState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->Resume();
	}
}


void GameEngine::HandleEvents()
{
	// let the state handle events
	states.back()->HandleEvents();
}

void GameEngine::Update()
{
	// Let the state update the game
	if (!states.empty())
        states.back()->Update();
}

void GameEngine::Draw()
{
	// Let the state draw the screen
	if (!states.empty()){
		states.back()->Draw();
        app.Display();
    }
}
