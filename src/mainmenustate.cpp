#include <SFML/Audio.hpp>

#include "gameengine.hpp"
#include "gamestate.hpp"
#include "mainmenustate.hpp"
#include "playstate.hpp"

MainMenuState MainMenuState::m_MainMenuState;

void MainMenuState::Init(GameEngine* game){
    mGameEngine = game;

    mInput = & (mGameEngine->app.GetInput());
    mGameEngine->app.SetFramerateLimit(60);

    uint16_t button_height = 30;
    uint16_t button_width = 120;
    uint16_t left = mGameEngine->app.GetWidth() / 2 - button_width/2;

    // Game Start Button
    gameStartButton = new cp::cpButton(&mGameEngine->app, &gui, "Start Game",left, 10, button_width, button_height);
    gameStartButton->SetFontSize(14);

    // Options Button
    optionsButton = new cp::cpButton(&mGameEngine->app, &gui, "Options",left, 45, button_width, button_height);
    optionsButton->SetFontSize(14);

    // Exit Button
    exitButton = new cp::cpButton(&mGameEngine->app, &gui, "Exit",left, 80, button_width, button_height);
    exitButton->SetFontSize(14);

    if(!m_MenuMusic.OpenFromFile("data/music/menu1.ogg")){
        std::cerr << "ERROR: Cound not load menu music." << std::endl;
    }
    m_MenuMusic.Play();
}

void MainMenuState::Cleanup(){
}

void MainMenuState::Pause(){
}

void MainMenuState::Resume(){
}

void MainMenuState::HandleEvents(){
    sf::Event Event;
	while (mGameEngine->app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			mGameEngine->Cleanup();
		}
        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
            mGameEngine->Cleanup();
        }

        // Game Start Button Click
        if(gameStartButton->CheckState(mInput) == cp::CP_ST_MOUSE_LBUTTON_RELEASED) {
            m_MenuMusic.Stop();
            mGameEngine->ChangeState( PlayState::Instance() );
        }

        // Options Button Click
        if(optionsButton->CheckState(mInput) == cp::CP_ST_MOUSE_LBUTTON_RELEASED) {
            optionsButton->SetLabelText("No options!");
        }

        // Exit Button Click
        if(exitButton->CheckState(mInput) == cp::CP_ST_MOUSE_LBUTTON_RELEASED) {
            m_MenuMusic.Stop();
            mGameEngine->Cleanup();
        }


        gui.ProcessKeys(&Event);
    }
}

void MainMenuState::Update(){
    sf::Sleep(0.001f); // for music thread
}

void MainMenuState::Draw(){
    // Clear Background
    mGameEngine->app.Clear(sf::Color(0,0,0));

    // Draw Menu
    gameStartButton->Draw();
    optionsButton->Draw();
    exitButton->Draw();
}

