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
	ResMgr.AddImage("data/images/", "block-sky.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-grass.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-dirt.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-lava.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-rock.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "tower-generic.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "highlight.svg", m_ratio, m_ratio);

	m_blocknbx = app.GetWidth() / m_ratio;
	m_blocknby = app.GetHeight() / m_ratio;

	std::cout << m_blocknbx << " " << m_blocknby << " " << m_ratio << std::endl;

	m_blocks.resize(extents[m_blocknbx+1][m_blocknby+1]);
	int i = 0;
	for (short unsigned int x = 0;x < m_blocknbx;++x) {
		for (short unsigned int y = 0;y <= m_blocknby;++y) {
            if ((x < (m_blocknbx - 20) / 2) or (x > (m_blocknbx - ((m_blocknbx - 20) / 2)-1 ))) {
            	// This is to create only m_blocks in the non playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-sky"), 1);
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-lava"), 0);
				} else {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-rock"), 0);
		        }
            } else {
            	// TODO: other graphics here, this is the playable area
            	if (y < 10) {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-sky"), 1);
				} else if (y == 10) {
				m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-grass"), 0);
				} else {
					m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("block-dirt"), 0);
		        }
            }
        }
			++i;
	}

    // TODO: Maybe use shared_ptr later on here?
	highlightblock = new Block(0, 0, ResMgr.GetImage("highlight"), 1);


	mode = 1; // set to build mode
}

void ClientApp::HandleInput() {
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
                    SendPacket << m_name << (sf::Uint16) (GetMouseBlock('x','p') - (m_width / m_ratio - 20)/2) << GetMouseBlock('y','p') << "build";
                    Socket.Send(SendPacket, m_bindaddress, m_port);
                    SendPacket.Clear();
                }
            }
		}
		if (Event.Type == sf::Event::MouseMoved) {
			SendPacket << m_name << (sf::Uint16) input->GetMouseX() << (sf::Uint16) input->GetMouseY() << "rolf";
			Socket.Send(SendPacket, m_bindaddress, m_port);
			SendPacket.Clear();
		}
	}
}

void ClientApp::Update() {


	// Network tests
    // std::cout << input->GetMouseX() << " - " << input->GetMouseY() << "\n";
	//SendPacket << (sf::Uint16)input->GetMouseX() << (sf::Uint16)input->GetMouseY();
	sf::Uint16 actionid;
	sf::Uint16 posx = 0;
	sf::Uint16 posy = 0;
    Network::IPAddress svaddress;
    unsigned short svport;
    if (Socket.Receive(RecvPacket, svaddress, svport)) {
    RecvPacket >> actionid >> posx >> posy;
    if (actionid == 1) {
        PlaceBlock((int) posx, (int) posy);
    }
    std::cout << actionid << " " << posx << " " << posy << std::endl;
    RecvPacket.Clear();
    }
}

void ClientApp::Draw() {
	uint16_t framerate = (uint16_t) (1.f / app.GetFrameTime());

	for (short unsigned int x = 0;x<m_blocknbx;++x) {
		for (short unsigned int y = 0;y<m_blocknby;++y) {
			if (mode == 1) {
				app.Draw(m_blocks[x][y]->Sprite);
			}
		}
	}

	highlightblock->SetPos((float)GetMouseBlock('x', 'a')*m_ratio, (float)GetMouseBlock('y','a')*m_ratio);
	app.Draw(highlightblock->Sprite);



	#ifdef DEBUG
	app.Draw(inforect);
		fps.SetText("FPS: "+boost::lexical_cast<std::string>(framerate));
	fps.SetPosition(10, app.GetHeight() - 40);
	app.Draw(fps);
		mousepos.SetText("MouseX: "+boost::lexical_cast<std::string>(GetMouseBlock('x','p'))+
                       	" MouseY: "+boost::lexical_cast<std::string>(GetMouseBlock('y','p')));
	mousepos.SetPosition(150, app.GetHeight() - 40);
	app.Draw(mousepos);
	#endif
	app.Display();
}

void ClientApp::Die() {
    Socket.Close();
}

sf::Uint16 ClientApp::GetMouseBlock(char xy, char type) {
    if (type == 'p') { //this stands for playable area
        if (xy == 'x') {
            return (sf::Uint16) ((input->GetMouseX() / m_ratio) - ((m_width - 20*m_ratio)/2) / m_ratio);
        } else {
            return (sf::Uint16) (input->GetMouseY() / m_ratio);
        }
    } else {
        if (xy == 'x') {
            return (sf::Uint16) (input->GetMouseX() / m_ratio);
        } else {
            return (sf::Uint16) (input->GetMouseY() / m_ratio);
        }
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

bool ClientApp::InPlayableArea(int x, int y) {
    // i was to lazy to implement this at that time i need it
    return true;
    //if ( () or () ) {
    //}
}

void ClientApp::PlaceBlock(int x, int y) {
    if (!InPlayableArea(x,y)) {
        std::cout << "Cannot place the block right there!";
    } else {
        if (m_blocks[x][y+1]->m_type != 1) {
            m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("tower-generic"), 2);
            std::cout << "lol" << std::endl;
        } else {
            std::cout << m_blocks[x][y-1]->m_type << std::endl << m_blocks[x][y]->m_type << std::endl;
        }
    }
    return;
}

Network::Socket& ClientApp::GetSocket() {
	return Socket;
}
