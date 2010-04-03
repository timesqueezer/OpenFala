#include "gameengine.hpp"
#include "gamestate.hpp"
#include "playstate.hpp"
#include "mainmenustate.hpp"


PlayState PlayState::m_PlayState;

void PlayState::Init(GameEngine* game){
    std::cout << "Init PlayState" << std::endl;

    mInput = & (game->app.GetInput());
    game->app.SetFramerateLimit(60);
}

void PlayState::Cleanup(){
}

void PlayState::Pause(){
}

void PlayState::Resume(){
}

void PlayState::HandleEvents(GameEngine* game){
    sf::Event Event;
	while (game->app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			game->Cleanup();
		}
        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
            // if Escape is pressed, the Main Menu is shown
           	game->ChangeState( MainMenuState::Instance() );
        }
	}
}

void PlayState::Update(GameEngine* game){

}

void PlayState::Draw(GameEngine* game){
    // Clear the screen
    game->app.Clear(sf::Color(255,0,0));
}

