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
/*
// Big constructor!
ClientApp::ClientApp(const uint16_t& port, const Network::IPAddress& bind_ad,
						const std::string& size, const std::string& name) {
	m_width = Utility::splitSize(size)[0];
	m_height = Utility::splitSize(size)[1];
	m_bindaddress = bind_ad;
	m_port = port; // port for outgoing communication (default 41311)
    m_clientport = 41312; // port for incoming communication
	m_name = name;

	inforect = sf::Shape::Rectangle(0, app.GetHeight() - 40, app.GetWidth(),
                                 app.GetHeight(), sf::Color(20, 20, 20));
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
	ResMgr = ResourceManager::ResourceManager();
	ResMgr.AddImage("data/images/", "block-sky.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-grass.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-dirt.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-lava.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "block-rock.svg", m_ratio, m_ratio);
	ResMgr.AddImage("data/images/", "tower-generic.svg", m_ratio, m_ratio);



	m_blocknbx = app.GetWidth() / m_ratio;
	m_blocknby = app.GetHeight() / m_ratio;


	std::cout << m_blocknbx << " " << m_blocknby << " " << m_ratio << std::endl;

    m_mpos.resize(extents[4]);

    //Initialisation of the Shapes to show each player
    for (short unsigned int i = 0; i < 4; ++i) {
        m_mpos[i] = new Block(0, 0, sf::Shape::Rectangle(0.f, 0.f, m_ratio, m_ratio, sf::Color(255,128,0,128), 1.f, sf::Color(255,128,255,128)));
    }

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

    m_cl_id = 0;
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
                    SendPacket << m_cl_id << (sf::Uint16) (GetMouseBlock('x','p') - (m_width / m_ratio - 20)/2) << GetMouseBlock('y','p') << "block" << m_name;
                    Socket.Send(SendPacket, m_bindaddress, m_port);
                    SendPacket.Clear();
                }
            }
		}
		if (Event.Type == sf::Event::MouseMoved) {
		    float mouse_x = (m_mpos[0]->m_Shape.GetPosition().x)/m_ratio;
		    float mouse_y = (m_mpos[0]->m_Shape.GetPosition().y)/m_ratio;

            if ((GetMouseBlock('x', 'p') != mouse_x) or (GetMouseBlock('y', 'p') != mouse_y)) {
		    	SendPacket << m_cl_id << GetMouseBlock('x', 'p') << GetMouseBlock('y', 'p') << "mouse" << m_name;
		    	Socket.Send(SendPacket, m_bindaddress, m_port);
		    	SendPacket.Clear();
		    	m_mpos[0]->m_Shape.SetPosition(GetMouseBlock('x', 'p')*m_ratio, GetMouseBlock('y', 'p')*m_ratio);
		    }
		}

	}
}

void ClientApp::Update() {
	sf::Uint16 actionid = 5;
	sf::Uint16 posx = 0;
	sf::Uint16 posy = 0;
	sf::Uint16 cl_id = 0;
    Network::IPAddress svaddress;
    unsigned short svport;
    if (Socket.Receive(RecvPacket, svaddress, svport) == sf::Socket::Done) {
        RecvPacket >> actionid >> posx >> posy >> cl_id;
        std::cout << "Server sent following Package: " << actionid << " (actionid), " << posx << " (X), " << posy << " (Y), " << cl_id << " (PlayerID)" << std::endl;
        if (actionid == 1) { // stands for placing a block
            PlaceBlock((int) posx, (int) posy);
        }
        if (actionid == 2) { //stands for getting the mouse positions
            m_mpos[cl_id]->m_Shape.SetPosition((float) posx*m_ratio, (float) posy*m_ratio);
        }
        if (actionid == 3) { // init stuff
            m_cl_id = cl_id;
        }
        //std::cout << actionid << " " << posx << " " << posy << std::endl;
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

    for (short unsigned int x = 0; x < 4; ++x) {
        app.Draw(m_mpos[x]->m_Shape);
    }


	app.Draw(inforect);
    fps.SetText("FPS: "+boost::lexical_cast<std::string>(framerate));
	fps.SetPosition(10, app.GetHeight() - 40);
	app.Draw(fps);
		mousepos.SetText("MouseX: "+boost::lexical_cast<std::string>(GetMouseBlock('x','p'))+
                       	" MouseY: "+boost::lexical_cast<std::string>(GetMouseBlock('y','p'))+
                        " BlockX: "+boost::lexical_cast<std::string>(m_mpos[0]->m_Shape.GetPosition().x)+
                        " BlockY: "+boost::lexical_cast<std::string>(m_mpos[0]->m_Shape.GetPosition().y));
	mousepos.SetPosition(150, app.GetHeight() - 40);
	app.Draw(mousepos);

	app.Display();
}

void ClientApp::Die() {
    Socket.Close();
}

sf::Uint16 ClientApp::GetMouseBlock(char xy, char type) {
    sf::Uint16 block;
    if (type == 'p') { //this stands for playable area
        if (xy == 'x') {
            block = ((input->GetMouseX() / m_ratio) - ((m_width - 20*m_ratio)/2) / m_ratio);
        } else {
            block = (input->GetMouseY() / m_ratio);
        }
    } else {
        if (xy == 'x') {
            block = (input->GetMouseX() / m_ratio);
        } else {
            block = (input->GetMouseY() / m_ratio);
        }
    }
    return block;
}

bool ClientApp::MouseInPlayableArea() {
    return InPlayableArea(input->GetMouseX(), input->GetMouseY());
}

bool ClientApp::InPlayableArea(int x, int y) {
    bool is_left_of_area  = x < ((m_width-20*m_ratio)/2) ;
    bool is_right_of_area = x > ((m_width-(m_width - 20*m_ratio)/2));
    bool is_underground   = y > 10*m_ratio;

    return !(is_left_of_area or is_right_of_area or is_underground);
}

void ClientApp::PlaceBlock(int x, int y) {
    if (!InPlayableArea(x,y)) {
        std::cout << "Cannot place the block right there!";
    } else {
        if (m_blocks[x][y+1]->m_type != 1) {
            m_blocks[x][y] = new Block(x*m_ratio, y*m_ratio, ResMgr.GetImage("tower-generic"), 2);
        } else {
            std::cout << "Can't place blocks in the air." << std::endl;
        }
    }
    return;
}

Network::Socket& ClientApp::GetSocket() {
	return Socket;
}
*/
