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


Client::Client(const uint16_t& p, const network::IPAddress& bind_ad, const std::string& size) {
	// app.Create(sf::VideoMode::GetMode(0), "OpenFala", 0,
	// sf::WindowSettings::WindowSettings ( 24, 8, 4));
	width = utility::splitSize(size)[0];
	height = utility::splitSize(size)[1];
	app.Create(sf::VideoMode(width, height, 32), "OpenFala");
	input = &app.GetInput();
	bind_address = bind_ad;
	port = p;

	#ifdef DEBUG
    sf::Shape debugcircle;
	debugcircle = sf::Shape::Circle(500, 500, 50, sf::Color(0, 0, 255));
	sf::Shape inforect;
	inforect = sf::Shape::Rectangle(0, app.GetHeight() - 40, app.GetWidth(),
                                 app.GetHeight(), sf::Color(20, 20, 20));
	sf::String mousepos;
	#endif

	app.SetFramerateLimit(60);
	filesystem::path path = filesystem::get_cwd();
	utility::ResourceLoader Loader = utility::ResourceLoader::ResourceLoader();
	Loader.AddImage(operator/(path, "data/images/").string(), "block-rock.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-sky.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-grass.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-dirt.png", 40, 40);
	Loader.AddImage(operator/(path, "data/images/").string(), "test.png", 40, 40);

	int blocknbx = app.GetWidth() / 40;
	int blocknby = app.GetHeight() / 40;

	typedef boost::multi_array<Block*, 2> array_type;
	array_type blocks(boost::extents[blocknbx][blocknby]);
	int i = 0;
	for (short unsigned int x = 0;x<blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
			if (y < 10) {
				blocks[x][y] = new Block(x*40, y*40, Loader.GetImage("block-sky"));
			} else if (y == 10) {
			blocks[x][y] = new Block(x*40, y*40, Loader.GetImage("block-grass"));
			} else {
				blocks[x][y] = new Block(x*40, y*40, Loader.GetImage("block-rock"));
            }
        }
			++i;
		}
	while (app.IsOpened()) {
		uint16_t framerate = (uint16_t) (1.f / app.GetFrameTime());
		sf::Event Event;
		while (app.GetEvent(Event)) {
			if (Event.Type == sf::Event::Closed) {
				app.Close();
			}

			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
            	app.Close();
            }
        }
		for (short unsigned int x = 0;x<blocknbx;++x) {
			for (short unsigned int y = 0;y<blocknby;++y) {
				app.Draw(blocks[x][y]->Sprite);
			}
		}

		// Network tests

		//std::cout << client.input->GetMouseX() << " - " << client.input->GetMouseY() << "\n";
		//client.packet << (sf::Uint16)client.input->GetMouseX() << (sf::Uint16)client.input->GetMouseY();
		//client.socket.Send(client.packet, client.bind_address, client.port);
		//client.packet.Clear();

		#ifdef DEBUG
			app.Draw(inforect);
			app.Draw(debugcircle);
			debugcircle.Move(0.5f, 0.5f);

			fps.SetText("FPS: "+boost::lexical_cast<std::string>(framerate));
			fps.SetPosition(10, app.GetHeight() - 40);
			app.Draw(fps);

			mousepos.SetText("MouseX: "+boost::lexical_cast<std::string>(input->GetMouseX())+
                          	" MouseY: "+boost::lexical_cast<std::string>(input->GetMouseY()));
			mousepos.SetPosition(150, app.GetHeight() - 40);
			app.Draw(mousepos);
		#endif
		app.Display();
	}
}

Client::~Client() {}
