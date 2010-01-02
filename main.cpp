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

namespace po = boost::program_options;

void startServer(void *UserData) {
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    uint16_t port = vm["port"].as<uint16_t>();

    std::cout << port << std::endl;
    sf::IPAddress address = sf::IPAddress::LocalHost;
    Server server(port);
    int i = 0;
    sf::Socket::Status status;
    server.Socket.SetBlocking(true);

    while (true) {
        server.Packet >> i;
        status = server.Socket.Receive(server.Packet, address, port);
        std::cout << "received: " << i << " packet: " << i <<
        " with status: " << status << std::endl << std::endl;
        if (!server.Packet >> i)
            std::cout << "kaputt";
    }
}

void clientLoop(void *UserData) {
    po::variables_map vm = *static_cast<po::variables_map*>(UserData);
    uint16_t port = vm["port"].as<uint16_t>();
    network::IPAddress address(vm["address"].as<std::string>());

    Client client(port, address);
	client.create_window();
	client.Circle = sf::Shape::Circle(500, 500, 50, sf::Color(0, 0, 255));
	client.App.SetFramerateLimit(60);
	filesystem::path path = filesystem::get_cwd();
	utility::ResourceLoader Loader = utility::ResourceLoader::ResourceLoader();
	Loader.AddImage(operator/(path, "data/images/").string(), "block-rock.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-sky.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-grass.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-dirt.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "test.png", 40, 40);

	int blocknbx = client.App.GetWidth() / 40;
	int blocknby = client.App.GetHeight() / 40;

	typedef boost::multi_array<Block*, 2> array_type;
	array_type blocks(boost::extents[blocknbx][blocknby]);
	int i = 0;
	for (short unsigned int x = 0;x<blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
			if (y < 15) {
				blocks[x][y] = new Block(x*40, y*40, Loader.GetImage("block-sky"));
			} else if (y == 15) {
			blocks[x][y] = new Block(x*40, y*40, Loader.GetImage("block-grass"));
			} else {
				if (i % 3 == 0) {
					blocks[x][y] = new Block(x*40, y*40, Loader.GetImage("block-rock"));
				} else {
					blocks[x][y] = new Block(x*40, y*40, Loader.GetImage("block-dirt"));
				}
			}
			++i;
		}
	}

	while (client.App.IsOpened()) {
		float framerate = 1.f / client.App.GetFrameTime();
		client.FPS.SetText(boost::lexical_cast<std::string>(framerate));
		sf::Event Event;
		while (client.App.GetEvent(Event)) {
			if (Event.Type == sf::Event::Closed) {
				client.App.Close();
			}

			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
            	client.App.Close();
            }
        }
		for (short unsigned int x = 0;x<blocknbx;++x) {
			for (short unsigned int y = 0;y<blocknby;++y) {
				client.App.Draw(blocks[x][y]->Sprite);
			}
		}

		client.App.Draw(client.Circle);
		client.Circle.Move(0.5f, 0.5f);
		client.App.Draw(client.FPS);
		client.App.Display();

	}
}

po::variables_map usage ( int& ac, char *av[] ) {
    // Declare the supported options.
    po::options_description desc("OpenFala multiplayer tower strategy game.\
    \n\nAllowed options");
    desc.add_options()
        ("help,h", "this message, obviously")
        ("mode,m", po::value<std::string>()->default_value("client"),
            "choose mode <modes: 'client', 'server', 'both'>")
        ("address,ip", po::value<std::string>()->default_value("empty"),
        	"get to connect to (ignored for server) <format: xxx.xxx.xxx.xxx>")
        ("port,p", po::value<uint16_t>()->default_value(1337),
        	"port to bind to")
        //("verbose,v", po::value<uint16_t>()->implicit_value(1), "be verbose")
        //("quiet,q", po::value<uint16_t>()->implicit_value(1), "be quiet")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(1);
    }
    /*
    if (vm.count("client") == vm.count("server") == vm.count("both") == 0) {
    	std::cout << "ERROR: You need to provide a valid mode!";
    	exit(1);
    }
	*/
    //
    //std::string address = vm["address"].as<std::string>();
	//if (vm["address"].as<std::string>() == "empty") {
	//	vm["address"].as<std::string>() = sf::IPAddress::LocalHost.ToString();
	//}

	sf::IPAddress address(vm["address"].as<std::string>());
	if (!address.IsValid()) {
		std::cout << "ERROR: Invalid address! " << std::endl;
	}

	//if (! vm.at(vm["address"].as<string>()

    return vm;
}

int main(int argc, char *argv[]) {
	po::variables_map opts = usage ( argc, argv );

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
