#include <SFML/System.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>
#include <boost/scoped_ptr.hpp>

#include <iostream>
#include <string>

#include "client.hpp"
#include "server.hpp"
#include "network.hpp"
#include "block.hpp"
#include "utility.hpp"

// Big constructor! Will have to do until we get scripting support in. Sorry.
ClientApp::ClientApp(const uint16_t& p, const Network::IPAddress& bind_ad,
						const std::string& size, const std::string& name) {
	// app.Create(sf::VideoMode::GetMode(0), "OpenFala", 0,
	// sf::WindowSettings::WindowSettings ( 24, 8, 4));
	width = Utility::splitSize(size)[0];
	height = Utility::splitSize(size)[1];
	app.Create(sf::VideoMode(width, height, 32), "OpenFala");
	input = &app.GetInput();
	bind_address = bind_ad;
	port = p;
	m_name = name;

	#ifdef DEBUG
    debugcircle = sf::Shape::Circle(100, 100, 50, sf::Color(128, 192, 255));
	inforect = sf::Shape::Rectangle(0, app.GetHeight() - 40, app.GetWidth(),
                                 app.GetHeight(), sf::Color(20, 20, 20));
	#endif

	app.SetFramerateLimit(60);

	ratio = height / 15;

	// Load images
	path = Filesystem::get_cwd();
	ResMgr = ResourceManager::ResourceManager();
	ResMgr.AddImage("data/images/", "block-rock.svg", ratio, ratio);
	ResMgr.AddImage("data/images/", "block-sky.svg", ratio, ratio);
	ResMgr.AddImage("data/images/", "block-grass.svg", ratio, ratio);
	ResMgr.AddImage("data/images/", "block-dirt.svg", ratio, ratio);
	ResMgr.AddImage("data/images/", "block-lava.svg", ratio, ratio);
	ResMgr.AddImage("data/images/debug/", "sky.svg", ratio, ratio);
	ResMgr.AddImage("data/images/debug/", "grass.svg", ratio, ratio);
	ResMgr.AddImage("data/images/debug/", "ground.svg", ratio, ratio);
	ResMgr.AddImage("data/images/debug/", "sky-out.svg", ratio, ratio);
	ResMgr.AddImage("data/images/debug/", "grass-out.svg", ratio, ratio);
	ResMgr.AddImage("data/images/debug/", "ground-out.svg", ratio, ratio);
	ResMgr.AddImage("data/images/debug/", "highlight.svg", ratio, ratio);

	blocknbx = app.GetWidth() / ratio;
	blocknby = app.GetHeight() / ratio;
	std::cout << blocknbx << " " << blocknby << " " << ratio << std::endl;

	m_blocks.resize(extents[blocknbx][blocknby]);
	int i = 0;
	for (short unsigned int x = 0;x < blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
            if ((x < (blocknbx - 20) / 2) or (x > (blocknbx - ((blocknbx - 20) / 2) ))) {
            	// This is to create only m_blocks in the non playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, ResMgr.GetImage("sky-out"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*ratio, y*ratio, ResMgr.GetImage("grass-out"));
				} else {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, ResMgr.GetImage("ground-out"));
		        }
            } else {
            	// TODO: other graphics here, this is the playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, ResMgr.GetImage("sky"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*ratio, y*ratio, ResMgr.GetImage("grass"));
				} else {
					m_blocks[x][y] = new Block(x*ratio, y*ratio, ResMgr.GetImage("ground"));
		        }
            }
        }
			++i;
	}

    // TODO: Maybe use shared_ptr later on here?
	boost::scoped_ptr<Block> highlightblock(new Block(0, 0, ResMgr.GetImage("highlight")));

	mode = 1; // set to build mode
}

ClientApp::~ClientApp() {}

void ClientApp::Init() {
}

void ClientApp::HandleInput() {
}

void ClientApp::Update() {
    sf::Event Event;
	while (app.GetEvent(Event)) {
		if (Event.Type == sf::Event::Closed) {
			app.Close();
		}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape)) {
           	app.Close();
           }
		if (Event.Type == sf::Event::MouseButtonPressed) {
            if (mode==1) {
                if (MouseInPlayableArea()) {
                    Packet << m_name << (sf::Uint16) (GetMouseBlock('x') - (width / ratio - 20)/2) << GetMouseBlock('y') << "rolf";
                    Socket.Send(Packet, bind_address, port);
                    Packet.Clear();
                }
            }
		}
		if (Event.Type == sf::Event::MouseMoved) {
			Packet << m_name << (sf::Uint16) input->GetMouseX() << (sf::Uint16) input->GetMouseY() << "mouse";
			Socket.Send(Packet, bind_address, port);
			Packet.Clear();
		}
	}

	// Network tests
    // std::cout << input->GetMouseX() << " - " << input->GetMouseY() << "\n";
	//Packet << (sf::Uint16)input->GetMouseX() << (sf::Uint16)input->GetMouseY();
	sf::Uint16 playerid;
	sf::Uint16 posx;
	sf::Uint16 posy;
    Network::IPAddress svaddress;
    unsigned short svport;
    GetSocket().Receive(Packet, svaddress, svport);
    Packet >> playerid >> posx >> posy;
    std::cout << playerid << " rolf " << posx << " " << posy << std::endl;
    Packet.Clear();
}

void ClientApp::Draw() {
	uint16_t framerate = (uint16_t) (1.f / app.GetFrameTime());

	for (short unsigned int x = 0;x<blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
			if (mode == 1) {
				app.Draw(m_blocks[x][y]->Sprite);
			}
		}
	}
	highlightblock->SetPos((float)GetMouseBlock('x')*ratio, (float)GetMouseBlock('y')*ratio);
	app.Draw(highlightblock->Sprite);

	#ifdef DEBUG
	app.Draw(inforect);
	app.Draw(debugcircle);
	debugcircle.Move(0.5f, 0.5f);
		fps.SetText("FPS: "+boost::lexical_cast<std::string>(framerate));
	fps.SetPosition(10, app.GetHeight() - 40);
	app.Draw(fps);
		mousepos.SetText("MouseX: "+boost::lexical_cast<std::string>(GetMouseBlock('x') - (width / ratio - 20)/2)+
                       	" MouseY: "+boost::lexical_cast<std::string>(GetMouseBlock('y')));
	mousepos.SetPosition(150, app.GetHeight() - 40);
	app.Draw(mousepos);
	#endif
	app.Display();
}

sf::Uint16 ClientApp::GetMouseBlock(char xy) {
	if (xy == 'x') {
		return (sf::Uint16) (input->GetMouseX() / ratio);
	} else {
		return (sf::Uint16) (input->GetMouseY() / ratio);
	}
}

bool ClientApp::MouseInPlayableArea() {
    if ((input->GetMouseX()) < ((width-20*ratio)/2) or
       ((input->GetMouseX()) > ((width-(width - 20*ratio)/2)))) {
        #ifdef DEBUG
            std::cout << "false" << std::endl;
        #endif
        return false;
    }
    if ( input->GetMouseY() > 10*ratio ) {
        #ifdef DEBUG
            std::cout << "false" << std::endl;
        #endif
        return false;
    }
    return true;
}

Network::Socket& ClientApp::GetSocket() {
	return Socket;
}
