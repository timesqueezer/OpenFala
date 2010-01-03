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


ClientApp::ClientApp(const uint16_t& p, const Network::IPAddress& bind_ad, const std::string& size) {
	// app.Create(sf::VideoMode::GetMode(0), "OpenFala", 0,
	// sf::WindowSettings::WindowSettings ( 24, 8, 4));
	width = utility::splitSize(size)[0];
	height = utility::splitSize(size)[1];
	app.Create(sf::VideoMode(width, height, 32), "OpenFala");
	input = &app.GetInput();
	bind_address = bind_ad;
	port = p;

	#ifdef DEBUG
    
    debugcircle = sf::Shape::Circle(500, 500, 50, sf::Color(0, 0, 255));
	inforect = sf::Shape::Rectangle(0, app.GetHeight() - 40, app.GetWidth(),
                                 app.GetHeight(), sf::Color(20, 20, 20));
	#endif

	app.SetFramerateLimit(60);

	float ratio = height / 15;
	
	// Loading Images
	
	path = filesystem::get_cwd();
	Loader = utility::ResourceLoader::ResourceLoader();
	Loader.AddImage(operator/(path, "data/images/").string(), "block-rock.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-sky.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-grass.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "block-dirt.png", ratio, ratio);
	Loader.AddImage(operator/(path, "data/images/").string(), "test.png", ratio, ratio);
	//Loader.AddImage(operator/(path, "data/images/debug/").string(), "sky.svg", ratio, ratio);

	blocknbx = app.GetWidth() / ratio;
	blocknby = app.GetHeight() / ratio;
	std::cout << blocknbx << " " << blocknby << " " << ratio;

	m_blocks.resize(extents[blocknbx][blocknby]);
	int i = 0;
	for (short unsigned int x = 0;x < blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
            
            if ((x < (blocknbx - 20) / 2) or (x > (blocknbx - ((blocknbx - 20) / 2) ))) {
            	// This is to create only m_blocks in the non playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("block-sky"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("block-grass"));
				} else {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("block-rock"));
		        }
            	
            } else {
            	// TODO: other graphics here, this is the playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("block-rock"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("block-grass"));
				} else {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, Loader.GetImage("block-sky"));
		        }
            }
        }
			++i;
	}
}

ClientApp::~ClientApp() {}

void ClientApp::Update() {
	sf::Event Event;
	while (app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			app.Close();
		}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
           	app.Close();
           }
       }
	
	
		// Network tests
		std::cout << input->GetMouseX() << " - " << input->GetMouseY() << "\n";
		Packet << (sf::Uint16)input->GetMouseX() << (sf::Uint16)input->GetMouseY();
		Socket.Send(Packet, bind_address, port);
		Packet.Clear();
		
}

void ClientApp::Draw() {
	uint16_t framerate = (uint16_t) (1.f / app.GetFrameTime());
	for (short unsigned int x = 0;x<blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
			app.Draw(m_blocks[x][y]->Sprite);
		}
	}
	
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
