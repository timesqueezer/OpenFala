#include <SFML/System.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <string>

#include "client.hpp"
#include "server.hpp"
#include "network.hpp"
#include "block.hpp"
#include "utility.hpp"

// Big constructor!
ClientApp::ClientApp(const uint16_t& port, const Network::IPAddress& bind_ad,
						const std::string& size, const std::string& name) {
	m_width = Utility::splitSize(size)[0];
	m_height = Utility::splitSize(size)[1];
	m_bindaddress = bind_ad;
	m_port = port; // port for outgoing communication (default 41311)
    m_clientport = 41312; // port for incoming communication
	m_name = name;

	#ifdef DEBUG
    debugcircle = sf::Shape::Circle(100, 100, 50, sf::Color(128, 192, 255));
	inforect = sf::Shape::Rectangle(0, app.GetHeight() - 40, app.GetWidth(),
                                 app.GetHeight(), sf::Color(20, 20, 20));
	#endif
}

ClientApp::~ClientApp() {}

void ClientApp::Init() {
    app.Create(sf::VideoMode(m_width, m_height, 32), "OpenFala");
	input = &app.GetInput();

    Socket.Bind(m_clientport); // port for incoming communication
    Socket.SetBlocking(false);
	app.SetFramerateLimit(60);

	m_ratio = m_height / 15;

    // Load images
	path = Filesystem::get_cwd();
	ResMgr = ResourceManager::ResourceManager();
	ResMgr.AddImage("data/images/", "block-rock.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-sky.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-grass.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-dirt.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-lava.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/debug/", "sky.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/debug/", "grass.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/debug/", "ground.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/debug/", "sky-out.png", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/debug/", "grass-out.png", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/debug/", "ground-out.png", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/debug/", "highlight.png", m_ratio, m_ratio);

	blocknbx = app.GetWidth() / m_ratio;
	blocknby = app.GetHeight() / m_ratio;
	std::cout << blocknbx << " " << blocknby << " " << m_ratio << std::endl;

	m_blocks.resize(extents[blocknbx][blocknby]);
	int i = 0;
	for (short unsigned int x = 0;x < blocknbx;++x) {
		for (short unsigned int y = 0;y<blocknby;++y) {
            if ((x < (blocknbx - 20) / 2) or (x > (blocknbx - ((blocknbx - 20) / 2) ))) {
            	// This is to create only m_blocks in the non playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("sky-out"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("grass-out"));
				} else {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("ground-out"));
		        }
            } else {
            	// TODO: other graphics here, this is the playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("sky"));
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("grass"));
				} else {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("ground"));
		        }
            }
        }
			++i;
	}

    // TODO: Maybe use shared_ptr later on here?
	highlightblock = new Block(0, 0, ResMgr.GetImage("highlight"));

	mode = 1; // set to build mode
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
                    SendPacket << m_name << (sf::Uint16) (GetMouseBlock('x') - (m_width / m_ratio - 20)/2) << GetMouseBlock('y') << "rolf";
                    Socket.Send(SendPacket, m_bindaddress, m_port);
                    SendPacket.Clear();
                }
            }
		}
		if (Event.Type == sf::Event::MouseMoved) {
			SendPacket << m_name << (sf::Uint16) input->GetMouseX() << (sf::Uint16) input->GetMouseY() << "mouse";
			Socket.Send(SendPacket, m_bindaddress, m_port);
			SendPacket.Clear();
		}
	}

	// Network tests
    // std::cout << input->GetMouseX() << " - " << input->GetMouseY() << "\n";
	//SendPacket << (sf::Uint16)input->GetMouseX() << (sf::Uint16)input->GetMouseY();
	sf::Uint16 playerid;
	sf::Uint16 posx;
	sf::Uint16 posy;
    Network::IPAddress svaddress;
    unsigned short svport;
    Socket.Receive(RecvPacket, svaddress, svport);
    RecvPacket >> playerid >> posx >> posy;
    std::cout << playerid << " rolf " << posx << " " << posy << std::endl;
    RecvPacket.Clear();
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

	highlightblock->SetPos((float)GetMouseBlock('x')*m_ratio, (float)GetMouseBlock('y')*m_ratio);
	app.Draw(highlightblock->Sprite);

	#ifdef DEBUG
	app.Draw(inforect);
	app.Draw(debugcircle);
	debugcircle.Move(0.5f, 0.5f);
		fps.SetText("FPS: "+boost::lexical_cast<std::string>(framerate));
	fps.SetPosition(10, app.GetHeight() - 40);
	app.Draw(fps);
		mousepos.SetText("MouseX: "+boost::lexical_cast<std::string>(GetMouseBlock('x') - (m_width / m_ratio - 20)/2)+
                       	" MouseY: "+boost::lexical_cast<std::string>(GetMouseBlock('y')));
	mousepos.SetPosition(150, app.GetHeight() - 40);
	app.Draw(mousepos);
	#endif
	app.Display();
}

void ClientApp::Die() {
    Socket.Close();
}

sf::Uint16 ClientApp::GetMouseBlock(char xy) {
	if (xy == 'x') {
		return (sf::Uint16) (input->GetMouseX() / m_ratio);
	} else {
		return (sf::Uint16) (input->GetMouseY() / m_ratio);
	}
}

bool ClientApp::MouseInPlayableArea() {
    if ((input->GetMouseX()) < ((m_width-20*m_ratio)/2) or
       ((input->GetMouseX()) > ((m_width-(m_width - 20*m_ratio)/2)))) {
        #ifdef DEBUG
            std::cout << "false" << std::endl;
        #endif
        return false;
    }
    if ( input->GetMouseY() > 10*m_ratio ) {
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
