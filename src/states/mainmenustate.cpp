#include <SFML/Audio.hpp>

#include "gameengine.hpp"
#include "gamestate.hpp"
#include "mainmenustate.hpp"
#include "playstate.hpp"

MainMenuState MainMenuState::m_MainMenuState;

void MainMenuState::Init(GameEngine* game){
    std::cout << "+ Init MainMenuState" << std::endl;
    mGameEngine = game;

    mInput = & (mGameEngine->app.GetInput());
    mGameEngine->app.SetFramerateLimit(60);

    sf::Uint16 button_height = 100;
    sf::Uint16 button_width = 300;
    sf::Uint16 left = mGameEngine->app.GetWidth() / 2 - button_width/2;

    // Load and Init Background Image
    ResourceManager::get_mutable_instance().AddImage("data/images","title2.svg", mGameEngine->app.GetWidth(), mGameEngine->app.GetHeight());
    mBackgroundImage.SetImage(*ResourceManager::get_mutable_instance().GetImage("title2"));

    ResourceManager::get_mutable_instance().AddImage("data/images","menu_button_normal.svg", button_width, button_height);
    ResourceManager::get_mutable_instance().AddImage("data/images","menu_button_hover.svg", button_width, button_height);

    // Game Start Button
    gameStartButton = new cp::cpImageButton(&mGameEngine->app, &gui, ResourceManager::get_mutable_instance().GetImage("menu_button_normal") , "Start Game" ,left, 0);
    gameStartButton->SetHoverImage(ResourceManager::get_mutable_instance().GetImage("menu_button_hover") );
    gameStartButton->SetTextColor(sf::Color(255,195,21));
    gameStartButton->SetTextHoverColor(sf::Color(255,255,255));

    // Options Button
    optionsButton = new cp::cpImageButton(&mGameEngine->app, &gui, ResourceManager::get_mutable_instance().GetImage("menu_button_normal") , "Options" ,left, 100);
    optionsButton->SetHoverImage(ResourceManager::get_mutable_instance().GetImage("menu_button_hover") );
    optionsButton->SetTextColor(sf::Color(255,195,21));
    optionsButton->SetTextHoverColor(sf::Color(255,255,255));

    // Exit Button
    exitButton = new cp::cpImageButton(&mGameEngine->app, &gui, ResourceManager::get_mutable_instance().GetImage("menu_button_normal") , "Exit" ,left, 200);
    exitButton->SetHoverImage(ResourceManager::get_mutable_instance().GetImage("menu_button_hover") );
    exitButton->SetTextColor(sf::Color(255,195,21));
    exitButton->SetTextHoverColor(sf::Color(255,255,255));

    if (!mGameEngine->MusicPlaying())
        mGameEngine->StartMusic("data/music/menu1.ogg");

}

void MainMenuState::Cleanup(){
    std::cout << "- Cleanup MainMenuState" << std::endl;
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
            mGameEngine->StopMusic();
            mGameEngine->ChangeState( PlayState::Instance() );
        }

        // Options Button Click
        if(optionsButton->CheckState(mInput) == cp::CP_ST_MOUSE_LBUTTON_RELEASED) {
            optionsButton->SetLabelText("No options!");
        }

        // Exit Button Click
        if(exitButton->CheckState(mInput) == cp::CP_ST_MOUSE_LBUTTON_RELEASED) {
            mGameEngine->StopMusic();
            mGameEngine->Cleanup();
        }

        gui.ProcessKeys(&Event);
    }
}

void MainMenuState::Update(){
}

void MainMenuState::Draw(){
    // Clear Background
    mGameEngine->app.Clear(sf::Color(0,0,0));
    mGameEngine->app.Draw(mBackgroundImage);

    // Draw Menu
    gameStartButton->Draw();
    optionsButton->Draw();
    exitButton->Draw();
}

