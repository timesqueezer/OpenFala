#include <SFML/System.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>

#include <iostream>
#include <string>

#include "client.hpp"
#include "server.hpp"
#include "network.hpp"
#include "block.hpp"
#include "utility.hpp"

void startServer(void *UserData) {
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    uint16_t port = vm["port"].as<uint16_t>();

    Server server(port);
	while (true) {
		//Server.HandleRequest();
	}
}

void clientLoop(void *UserData) {
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    uint16_t port = vm["port"].as<uint16_t>();
    network::IPAddress address(vm["address"].as<std::string>());
    std::string size_str = vm["size"].as<std::string>();
    Client client(port, address, size_str);
}

int main(int argc, char *argv[]) {
	po::variables_map opts = utility::usage ( argc, argv );

	std::string mode = opts["mode"].as<std::string>();
	std::string address = opts["address"].as<std::string>();
	//uint16_t port = opts["port"].as<uint16_t>();

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
