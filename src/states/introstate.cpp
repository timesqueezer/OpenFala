#include "gameengine.hpp"
#include "gamestate.hpp"
#include "introstate.hpp"
#include "mainmenustate.hpp"

IntroState IntroState::m_IntroState;

void IntroState::Init(GameEngine* game){
    std::cout << "+ Init IntroState" << std::endl;
    mGameEngine = game;

    mInput = & (mGameEngine->app.GetInput());
    mGameEngine->app.SetFramerateLimit(60);

    // Create the Welcome Message
    m_welcome_message.SetText("Press Enter to get to the Main Menu");
    m_welcome_message.SetColor(sf::Color(0,0,0));
    // Center the message
    m_welcome_message.SetPosition(mGameEngine->app.GetWidth()/2 - m_welcome_message.GetRect().GetWidth()/2,
                                  mGameEngine->app.GetHeight()/2 - m_welcome_message.GetRect().GetHeight()/2);
}

void IntroState::Cleanup(){
    std::cout << "- Cleanup IntroState" << std::endl;
}

void IntroState::Pause(){
}

void IntroState::Resume(){
}

void IntroState::HandleEvents(){
    sf::Event Event;
	while (mGameEngine->app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			mGameEngine->Cleanup();
		}
        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Return)) {
           	mGameEngine->ChangeState( MainMenuState::Instance() );
        }
	}
}

void IntroState::Update(){
}

void IntroState::Draw(){
    mGameEngine->app.Clear(sf::Color(200,200,200));
    // draw welcome message
    mGameEngine->app.Draw(m_welcome_message);
}
