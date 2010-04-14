#include <SFML/System.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <string>

#include "server/server.hpp"
#include "block/block.hpp"
#include "utility/utility.hpp"
#include "states/gameengine.hpp"

#include "states/introstate.hpp"
#include "states/mainmenustate.hpp"
#include "states/playstate.hpp"

#define AREAX 20
#define AREAY 15


void startServer(void *UserData) {
    // We need to get back the passed program arguments here
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    sf::Uint16 port = vm["port"].as<sf::Uint16>();
	sf::Uint16 max_players = vm["maxplayers"].as<sf::Uint16>();

    // Create actual server in this thread
    ServerApp Server(port, max_players);
	Server.Init();
    sf::Clock Clock;
    const float tickrate = 30.f;
    const float timebudget = 1/tickrate;
    while (true) {
        while (Clock.GetElapsedTime() < timebudget) {
            Server.HandleRequest();
        }
        Clock.Reset();
		Server.Update();
	}
	Server.Die();
}

void clientLoop(void *UserData) {
    // We need to get back the passed program arguments here
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    sf::IPAddress address(vm["address"].as<std::string>());
    std::string size_str = vm["size"].as<std::string>();
	std::string name = vm["name"].as<std::string>();

	sf::Uint16 width = Utility::splitSize(size_str)[0];
	sf::Uint16 height = Utility::splitSize(size_str)[1];

    // Create and initialize GameEngine
    GameEngine game_engine(vm);
    game_engine.Init("OpenFala Client", width, height, false);

    // start the intro, which will then start other game states
    if (vm["mode"].as<std::string>() == "normal")
        game_engine.ChangeState(IntroState::Instance());
    else
        game_engine.ChangeState(PlayState::Instance());

    // Gameloop
    while (true) {
    	game_engine.HandleEvents();
    	if (!game_engine.Running())
            exit(0);
    	game_engine.Update();
    	game_engine.Draw();
    }
    // Close Window by cleaning up the game engine
    game_engine.Cleanup();
}

int main(int argc, char *argv[]) {

	po::variables_map opts = Utility::usage ( argc, argv );

	std::string mode = opts["mode"].as<std::string>();

	// TODO: Not needed here but left for reference
    //std::string address = opts["address"].as<std::string>();
	//sf::Uint16 port = opts["port"].as<sf::Uint16>();

    // Pass the program arguments along to both threads
	sf::Thread ServerThread(&startServer, &opts);
	sf::Thread ClientThread(&clientLoop, &opts);

	if (mode == "server") {
		ServerThread.Launch();
	}
	else if (mode == "client" or mode=="normal") {
		ClientThread.Launch();
	}
	else if (mode == "both") {
		ServerThread.Launch();
		sf::Sleep(2);
		ClientThread.Launch();
	}
	else {
		std::cout << "ERROR: Provide a valid mode!" << std::endl;
		exit(1);
	}

	ClientThread.Wait();
	ServerThread.Wait();
	return 0;
}
