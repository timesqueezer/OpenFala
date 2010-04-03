#include "gameengine.hpp"
#include "gamestate.hpp"
#include "introstate.hpp"
#include "mainmenustate.hpp"

IntroState IntroState::m_IntroState;

void IntroState::Init(GameEngine* game){
    mInput = & (game->app.GetInput());
    game->app.SetFramerateLimit(60);

    // Create the Welcome Message
    m_welcome_message.SetText("Press Enter to get to the Main Menu");
    m_welcome_message.SetColor(sf::Color(0,0,0));
    // Center the message
    m_welcome_message.SetPosition(game->app.GetWidth()/2 - m_welcome_message.GetRect().GetWidth()/2,
                                  game->app.GetHeight()/2 - m_welcome_message.GetRect().GetHeight()/2);
}

void IntroState::Cleanup(){
}

void IntroState::Pause(){
}

void IntroState::Resume(){
}

void IntroState::HandleEvents(GameEngine* game){
    sf::Event Event;
	while (game->app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			game->Cleanup();
		}
        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return)) {
           	game->ChangeState( MainMenuState::Instance() );
        }
	}
}

void IntroState::Update(GameEngine* game){
}

void IntroState::Draw(GameEngine* game){
    game->app.Clear(sf::Color(200,200,200));
    // draw welcome message
    game->app.Draw(m_welcome_message);
}
