#include <SFML/System.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <string>

#include "client.hpp"
#include "server.hpp"
#include "network.hpp"
#include "block.hpp"
#include "utility.hpp"

void startServer(void *UserData) {
    // We need to get back the passed program arguments here
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    uint16_t port = vm["port"].as<uint16_t>();
	uint16_t max_players = vm["maxplayers"].as<uint16_t>();

    // Create actual server in this thread
    ServerApp Server(port, max_players);
	Server.Init();
	while (true) {
		Server.HandleRequest();
		Server.Update();
	}
}

void clientLoop(void *UserData) {
    // We need to get back the passed program arguments here
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    uint16_t port = vm["port"].as<uint16_t>();
    Network::IPAddress address(vm["address"].as<std::string>());
    std::string size_str = vm["size"].as<std::string>();
	std::string name = vm["name"].as<std::string>();

    // Create actual client in this thread
    ClientApp Client(port, address, size_str, name);
    Client.Init();
    while (true) {
    	Client.HandleInput();
    	Client.Update();
    	Client.Draw();
    }
}

int main(int argc, char *argv[]) {
	po::variables_map opts = Utility::usage ( argc, argv );

	std::string mode = opts["mode"].as<std::string>();

	// TODO: Not needed here but left for reference
    //std::string address = opts["address"].as<std::string>();
	//uint16_t port = opts["port"].as<uint16_t>();

    // Pass the program arguments along to both threads
	sf::Thread ServerThread(&startServer, &opts);
	sf::Thread ClientThread(&clientLoop, &opts);

	if (mode == "server") {
		ServerThread.Launch();
	}
	else if (mode == "client") {
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
